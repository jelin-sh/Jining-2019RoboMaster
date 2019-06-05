//
// Created by Administrator on 2019/4/6.
//

#include "MotorControlSystem.h"
#include <ChassisControlUnit/ChassisControlUnit.h>
#include <CourtSystem.h>
#include <HolderControlUnit/HolderControlUnit.h>
#include <RemoteListener.h>
#include <ShootControlUnit/ShootControlUnit.h>
#include <cmsis_os.h>

osThreadId chassisThreadID;
osThreadId holderThreadID;
osThreadId shootThreadID;

void ChassisController(void const *argument);
void HolderController(void const *argument);
void ShootController(void const *argument);

void MotorControlSystem::Init() {
  ChassisControlUnit::Init();
  HolderControlUnit::Init();
  ShootControlUnit::Init();
  ShootControlUnit::OpenShootMotor();

  osThreadDef(chassisThread, ChassisController, osPriorityNormal, 0, 512);
  osThreadDef(holderThread, HolderController, osPriorityNormal, 0, 512);
  osThreadDef(shootThread, ShootController, osPriorityNormal, 0, 512);

  chassisThreadID = osThreadCreate(osThread(chassisThread), NULL);
  holderThreadID = osThreadCreate(osThread(holderThread), NULL);
  shootThreadID = osThreadCreate(osThread(shootThread), NULL);
}

void MotorControlSystem::UpdateCanParm() {
  ChassisControlUnit::UpdateCanParm();
  HolderControlUnit::UpdateCanParm();
  ShootControlUnit::UpdateCanParm();
}

void MotorControlSystem::UpdateControlParm() {
  if (RemoteListener::GetData().rc.s2 != 2) {
    /*TODO 此处为遥控器控制*/
    ChassisControlUnit::UpdateControlParmForRemote();
    HolderControlUnit::UpdateControlParmForRemote();
    ShootControlUnit::UpdateControlParmForRemote();

  } else {
    /*TODO 此处为PC控制*/
    ChassisControlUnit::UpdateControlParmForPC();
    HolderControlUnit::UpdateControlParmForPC();
    ShootControlUnit::UpdateControlParmForPC();
  }
}

void MotorControlSystem::RunChassis() {
  ChassisControlUnit::RunMotorForControlParm();
}

bool MotorControlSystem::ChassisOnline() {
  return ChassisControlUnit::IsOnline();
}

bool MotorControlSystem::HolderOnline() {
  return HolderControlUnit::IsOnline();
}

void MotorControlSystem::RunHolder() {
  HolderControlUnit::RunHolderForControlParm();
}

bool MotorControlSystem::ShootOnline() { return ShootControlUnit::IsOnline(); }

void MotorControlSystem::RunShoot() {
  ShootControlUnit::RunShootForControlParm();
}

void ChassisController(void const *argument) {
  for (;;) {
    if (MotorControlSystem::ChassisOnline()) {
      MotorControlSystem::RunChassis();
    }
    osDelay(5);
  }
}
void HolderController(void const *argument) {
  for (;;) {
    if (MotorControlSystem::HolderOnline()) {
      MotorControlSystem::RunHolder();
    }
    osDelay(1);
  }
}
void ShootController(void const *argument) {
  for (;;) {
    if (MotorControlSystem::ShootOnline()) {
      MotorControlSystem::RunShoot();
    }
    osDelay(5);
  }
}