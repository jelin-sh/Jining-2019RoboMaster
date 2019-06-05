//
// Created by Administrator on 2019/4/6.
//

#include "MotorControlSystem.h"
#include <ChassisControlUnit/ChassisControlUnit.h>
#include <CourtSystem.h>
#include <RemoteListener.h>
#include <TakeShotUnit/TakeShotUnit.h>
#include <UpIslandControlUnit/UpIslandControlUnit.h>
#include <cmsis_os.h>

osThreadId chassisThreadID;
osThreadId holderThreadID;
osThreadId shootThreadID;

void ChassisController(void const * argument);

void MotorControlSystem::Init()
{
  ChassisControlUnit::Init();
  UpIslandControlUnit::Init();
  TakeShotUnit::Init();

  osThreadDef(chassisThread,ChassisController,osPriorityNormal,0,512);
  chassisThreadID = osThreadCreate(osThread(chassisThread),NULL);
}

void MotorControlSystem::UpdateCanParm()
{
  ChassisControlUnit::UpdateCanParm();
  UpIslandControlUnit::UpdateMotorPramForCan();
  TakeShotUnit::UpdateMotorParmForCan();
}

void MotorControlSystem::UpdateControlParm()
{
  if(RemoteListener::GetData().rc.s2 != 2)
  {
    /*TODO 此处为遥控器控制*/
    ChassisControlUnit::UpdateControlParmForRemote();
    UpIslandControlUnit::UpdateControlPramForRemote();
    TakeShotUnit::UpdateControlPramForRemote();
  }
  else
  {
    /*TODO 此处为PC控制*/
    ChassisControlUnit::UpdateControlParmForPC();
    UpIslandControlUnit::UpdateControlPramForPC();
    TakeShotUnit::UpdateControlPramForPC();
  }

}

void MotorControlSystem::RunChassis()
{
  ChassisControlUnit::RunMotorForControlParm();
}

bool MotorControlSystem::ChassisOnline()
{
  return ChassisControlUnit::IsOnline();
}

void ChassisController(void const * argument)
{
  for(;;)
  {
    if(MotorControlSystem::ChassisOnline())
    {
      MotorControlSystem::RunChassis();
    }
    osDelay(5);
  }
}
