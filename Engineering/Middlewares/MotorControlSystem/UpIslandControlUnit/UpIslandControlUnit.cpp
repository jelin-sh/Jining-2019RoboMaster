//
// Created by jelin on 19-5-23.
//

#include "UpIslandControlUnit.h"
#include <C6020/C6020.h>
#include <CanBusManger.h>
#include <MyThread.h>
#include <RemoteListener.h>

void UpIslandController(void const *arg);
C6020 *islandMotor1;
C6020 *islandMotor2;
Port *switchUpIsland;
bool upIslandOn = false;
short islandMotorSpeed = 0;
void UpIslandControlUnit::Init() {
  islandMotor1 = new C6020(CAN_ID_Motor_7 | 0xf000,
                           new PID(10000, 1000, 0, 0, 2, 0.06, 2.5));
  islandMotor2 = new C6020(CAN_ID_Motor_8 | 0xf000,
                           new PID(10000, 1000, 0, 0, 2, 0.06, 2.5));
  auto UpIslandThread = new MyThread("UpIslandThread", 512, osPriorityNormal,
                                     UpIslandController, nullptr);
  switchUpIsland = new Port(GPIOD, GPIO_PIN_5);
  UpIslandThread->Run();
}
void UpIslandControlUnit::OpenMotor() { islandMotorSpeed = 1000; }
void UpIslandControlUnit::CloseMotor() { islandMotorSpeed = 0; }
bool UpIslandControlUnit::IsOnLand() { return false; }
void UpIslandControlUnit::UpdateMotorPramForCan() {
  islandMotor1->UpdateParm();
  islandMotor2->UpdateParm();
}
void UpIslandControlUnit::UpdateControlPramForRemote() {
  if (RemoteListener::GetData().rc.s1 == 1) {
    OpenMotor();
  } else {
    CloseMotor();
  }
}
void UpIslandControlUnit::UpdateControlPramForPC() {
  if (RemoteListener::HaveKey(KEY::KEY_Ctrl) &&
      !RemoteListener::LastHaveKey(KEY::KEY_Ctrl)) {
    // Enter Ctr
    upIslandOn = !upIslandOn;
  }
}

void UpIslandController(void const *arg) {
  for (;;) {
    if (islandMotorSpeed == 0) {
      islandMotor1->GetListener()->WriteData(0);
      islandMotor2->GetListener()->WriteData(0);
    } else {
      islandMotor1->GetListener()->WriteData(
          islandMotor1->CalcOutput(islandMotorSpeed));
      islandMotor2->GetListener()->WriteData(
          islandMotor2->CalcOutput(-islandMotorSpeed));
    }

    if (upIslandOn) {
      switchUpIsland->set();
      UpIslandControlUnit::OpenMotor();
    } else {
      switchUpIsland->reset();
      UpIslandControlUnit::CloseMotor();
    }
    osDelay(5);
  }
}
