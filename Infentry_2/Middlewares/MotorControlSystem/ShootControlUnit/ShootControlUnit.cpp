//
// Created by Administrator on 2019/4/30.
//

#include "ShootControlUnit.h"
#include <C6010/C6010.h>
#include <CanBusManger.h>
#include <RemoteListener.h>
#include <pwm/pwm.h>
#include <tim.h>

PWM *shootMotor1;
PWM *shootMotor2;
PWM *shootSwitch;
C6010 *pordMotor;
u32 shootSpeed = 2000;
bool shootControl = false;
bool shootConrtolRe = false;
bool shootGetShotSign = false;
void ShootControlUnit::Init() {
  shootMotor1 = new PWM(&htim8, TIM_CHANNEL_1);
  shootMotor2 = new PWM(&htim8, TIM_CHANNEL_2);
  shootSwitch = new PWM(&htim8, TIM_CHANNEL_3);
  pordMotor =
      new C6010(CAN_ID_SHOOT, new PID(10000, 1000, 0, 0, 1.5, 0.001, 2));

  shootSwitch->out(2450);
  shootSwitch->start();
}

void ShootControlUnit::OpenShootMotor() {
  HAL_Delay(5000);
  shootMotor1->out(800);
  shootMotor2->out(800);
  shootMotor1->start();
  shootMotor2->start();
  HAL_Delay(2000);
  for (int k = 800; k < shootSpeed; k++) {
    shootMotor1->out(k);
    shootMotor2->out(k);
  }
}

void ShootControlUnit::SetShootMotorSpeed(
    ShootControlUnit::ShootMotorSpeed speed) {
  switch ((int)speed) {
  case (int)ShootMotorSpeed::LOW:
    shootSpeed = 140;
    break;
  case (int)ShootMotorSpeed::MID:
    shootSpeed = 170;
    break;
  case (int)ShootMotorSpeed::HEIGH:
    shootSpeed = 200;
    break;
  }
}

void ShootControlUnit::On() {
  pordMotor->GetListener()->WriteData(pordMotor->CalcOutput(2000));
}

void ShootControlUnit::Off() { pordMotor->GetListener()->WriteData(0); }

void ShootControlUnit::UpdateCanParm() { pordMotor->UpdateParm(); }

void ShootControlUnit::UpdateControlParmForRemote() {
  shootControl = (RemoteListener::GetData().rc.s1 == 1);
  shootGetShotSign = (RemoteListener::GetData().rc.s1 == 2);
}

bool ShootControlUnit::IsOnline() {
  return pordMotor->GetListener()->isOnline();
}

void ShootControlUnit::RunShootForControlParm() {
  if (shootControl) {
    On();
  } else if (shootConrtolRe) {
    reOn();
  } else {
    Off();
  }

  if (shootGetShotSign) {
    shootSwitch->out(500);
  } else {
    shootSwitch->out(2450);
  }
}

void ShootControlUnit::UpdateControlParmForPC() {

  shootControl = RemoteListener::GetData().mouse.press_l == 1;
  shootConrtolRe = RemoteListener::GetData().mouse.press_r == 1;
  if (RemoteListener::LastHaveKey(KEY::KEY_E) &&
      !RemoteListener::HaveKey(KEY::KEY_E)) {
    shootGetShotSign = !shootGetShotSign;
  }
}
void ShootControlUnit::reOn() {
  pordMotor->GetListener()->WriteData(pordMotor->CalcOutput(-2000));
}
