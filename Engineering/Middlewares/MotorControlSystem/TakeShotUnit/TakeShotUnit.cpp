//
// Created by jelin on 19-5-24.
//

#include "TakeShotUnit.h"
#include <C6010/C6010.h>
#include <CanBusManger.h>
#include <MyThread.h>
#include <RemoteListener.h>
#include <cmath>
#include <cmsis_os.h>
#include <pwm/pwm.h>
#include <tim.h>

/*
 * OpenCloseArm:  PI0 --- A
 * OutBackArm:    PH12 -- B
 * TakeSwitch:    PH11 -- C
 * L              D
 * */

C6010 *upDownMotor1;
C6010 *upDownMotor2;
C6020 *armMotor;

Port *AirOpenCloseArm;
Port *AirOutBackArm;
Port *AirTakeSwitch;
Port *AirLeave;

Port *SwitchUpDown1;
Port *SwitchUpDown2;

PWM *holderHead;

u8 countCMD = 0;
short armUpDownSpeed = 0;
short armGetSpeed = 0;

static bool armUpStatus = false;
static bool armUpDownStatus = false;  // down
static bool armGetStatus = false;     // init
static bool armTakeOnStatus = false;  // init
static bool armLeaveOnStatus = false; // init
static bool armAutoTakeStatus = false;
static bool armOutBackStatus = false;

void TakeShotController(void const *arg);

void TakeShotUnit::Init() {
  upDownMotor1 =
      new C6010(CAN_ID_Motor_5, new PID(10000, 5000, 0, 0, 2, 0.01, 2));
  upDownMotor2 =
      new C6010(CAN_ID_Motor_6, new PID(10000, 5000, 0, 0, 2, 0.01, 2));
  armMotor = new C6020(CAN_ID_Motor_7, new PID(10000, 1000, 0, 0, 7, 0.1, 2.5));

  AirOutBackArm = new Port(GPIOI, GPIO_PIN_0);
  AirOpenCloseArm = new Port(GPIOH, GPIO_PIN_12);
  AirTakeSwitch = new Port(GPIOH, GPIO_PIN_11);
  AirLeave = new Port(GPIOH, GPIO_PIN_10);

  SwitchUpDown1 = new Port(GPIOI, GPIO_PIN_7);
  SwitchUpDown2 = new Port(GPIOI, GPIO_PIN_2);

  holderHead = new PWM(&htim8, TIM_CHANNEL_1);
  holderHead->out(1300);
  holderHead->start();

  auto TaskShotThread = new MyThread("TakeShotThread", 512, osPriorityNormal,
                                     TakeShotController, nullptr);

  TaskShotThread->Run();
  AirOpenCloseArm->reset();
  AirOutBackArm->reset();
  AirTakeSwitch->reset();
  AirLeave->reset();
}
void TakeShotUnit::UpdateMotorParmForCan() {
  upDownMotor1->UpdateParm();
  upDownMotor2->UpdateParm();
  armMotor->UpdateParm();
}
void TakeShotUnit::UpdateControlPramForRemote() {}
void TakeShotUnit::UpdateControlPramForPC() {
  if (RemoteListener::HaveKey(KEY::KEY_Q) &&
      !RemoteListener::LastHaveKey(KEY::KEY_Q) &&
      !RemoteListener::HaveKey(KEY::KEY_Shift)) {
    armUpDownStatus = !armUpDownStatus;
    if (armUpDownStatus) {
      ArmUp();
      holderHead->out(680);
    } else {
      ArmDown();
      holderHead->out(1300);
    }
  }
  if (IsTakeMode()) {
    if (RemoteListener::HaveKey(KEY::KEY_Shift) &&
        RemoteListener::HaveKey(KEY::KEY_Q)) {
      ArmUp();
      armUpStatus = true;
    } else {
      armUpStatus = false;
    }
    if (RemoteListener::GetData().mouse.press_l &&
        !RemoteListener::GetLastData().mouse.press_l) {
      armGetStatus = !armGetStatus;
      if (armGetStatus) {
        ArmGetDown();
      } else {
        ArmGetUp();
      }
    }
    if (RemoteListener::HaveKey(KEY::KEY_E) &&
        RemoteListener::HaveKey(KEY::KEY_Shift)) {
      if (armGetStatus) {
        armAutoTakeStatus = true;
      }
    } else if (RemoteListener::HaveKey(KEY::KEY_E) &&
               !RemoteListener::LastHaveKey(KEY::KEY_E)) {
      armTakeOnStatus = !armTakeOnStatus;
      if (armTakeOnStatus) {
        ArmClose();
      } else {
        ArmOpen();
      }
    }
    if (RemoteListener::GetData().mouse.press_l &&
               RemoteListener::GetLastData().mouse.press_r) {
      if (!RemoteListener::GetLastData().mouse.press_r) {
        armOutBackStatus = !armOutBackStatus;
        if (armOutBackStatus) {
          AirOutBackArm->set();
        } else {
          AirOutBackArm->reset();
        }
      }
    }
  }
  else if (RemoteListener::HaveKey(KEY::KEY_Shift) &&
           RemoteListener::GetData().mouse.press_r) {
    if (!RemoteListener::GetLastData().mouse.press_r) {
      armLeaveOnStatus = !armLeaveOnStatus;
      if (armLeaveOnStatus) {
        ArmUp();
      } else {
        AirTakeSwitch->reset();
        ArmDown();
      }
    }
  }
  else if (RemoteListener::GetData().mouse.press_r &&
           !RemoteListener::GetLastData().mouse.press_r) {
    if(armLeaveOnStatus){
      AirTakeSwitch->set();
    }
  }
}
void TakeShotUnit::ArmUp() { armUpDownSpeed = 6000; }
void TakeShotUnit::ArmDown() { armUpDownSpeed = -4000; }
void TakeShotUnit::ArmStay() { armUpDownSpeed = 0; }
void TakeShotUnit::ArmOpen() { AirOpenCloseArm->set(); }
void TakeShotUnit::ArmClose() { AirOpenCloseArm->reset(); }
void TakeShotUnit::ArmOut() { AirOutBackArm->set(); }
void TakeShotUnit::ArmBack() { AirOutBackArm->reset(); }
void TakeShotUnit::ArmGetUp() { armGetSpeed = 2500; }
void TakeShotUnit::ArmTakeOn() { AirTakeSwitch->set(); }
void TakeShotUnit::ArmTakeOff() { AirTakeSwitch->reset(); }
void TakeShotUnit::ArmGetDown() { armGetSpeed = -3000; }
bool TakeShotUnit::IsTakeMode() { return armUpDownStatus; }
void TakeShotUnit::AutoTakeShot() {
  static u16 timeAuto = 0;
  if (armOutBackStatus) {
    AirOutBackArm->reset();
    armOutBackStatus = false;
  }
  ArmGetUp();
  timeAuto++;
  if (timeAuto > 100) {
    ArmGetDown();
    if (!(fabs(armMotor->GetSpeed()) < 10 &&
          fabs(armMotor->GetTorque()) > 3000)) {
      if (timeAuto > 140) {
        ArmOpen();
        armAutoTakeStatus = false;
        timeAuto = 0;
        ArmGetUp();
      }
    }
  }
}

void TakeShotController(void const *arg) {
  short motor1Output = 0;
  short motor2Output = 0;
  short armMotorOutput = 0;
  for (;;) {

    motor1Output = upDownMotor1->CalcOutput(armUpDownSpeed);
    motor2Output = upDownMotor2->CalcOutput(-armUpDownSpeed);
    armMotorOutput = armMotor->CalcOutput(armGetSpeed);

    if ((fabs(upDownMotor2->GetSpeed()) < 50 &&
         fabs(upDownMotor2->GetTorque()) > 4700) ||
        (fabs(upDownMotor1->GetSpeed()) < 50 &&
         fabs(upDownMotor1->GetTorque()) > 4700)) {
      motor1Output = 0;
      armUpDownSpeed = 0;
    }else if (SwitchUpDown2->get() == Port::Port_Status_Off) {
      if (!armUpStatus && armUpDownStatus) {
        armUpDownSpeed = 0;
      }
    }
    if (fabs(armMotor->GetSpeed()) < 10 && fabs(armMotor->GetTorque()) > 3000) {
      armGetSpeed = 0;
      armMotorOutput = 0;
    }
    if(armUpDownSpeed < 0){
      motor1Output = 0;
      motor2Output = 0;
    }
    upDownMotor1->GetListener()->WriteData(motor1Output);
    upDownMotor2->GetListener()->WriteData(-motor1Output);
    armMotor->GetListener()->WriteData(armMotorOutput);

    if (armAutoTakeStatus) {
      TakeShotUnit::AutoTakeShot();
    }

    osDelay(5);
  }
}