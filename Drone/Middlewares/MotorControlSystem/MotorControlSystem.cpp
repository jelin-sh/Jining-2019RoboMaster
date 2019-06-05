//
// Created by Administrator on 2019/4/6.
//

#include <HolderControlUnit/HolderControlUnit.h>
#include <ShootControlUnit/ShootControlUnit.h>
#include <RemoteListener.h>
#include <cmsis_os.h>
#include "MotorControlSystem.h"

osThreadId chassisThreadID;
osThreadId holderThreadID;
osThreadId shootThreadID;

void HolderController(void const * argument);
void ShootController(void const * argument);

void MotorControlSystem::Init()
{
  HolderControlUnit::Init();
  ShootControlUnit::Init();
  ShootControlUnit::OpenShootMotor();

  osThreadDef(holderThread,HolderController,osPriorityNormal,0,512);
  osThreadDef(shootThread,ShootController,osPriorityNormal,0,512);

  holderThreadID = osThreadCreate(osThread(holderThread),NULL);
  shootThreadID = osThreadCreate(osThread(shootThread),NULL);
}

void MotorControlSystem::UpdateCanParm()
{
  HolderControlUnit::UpdateCanParm();
  ShootControlUnit::UpdateCanParm();
}

void MotorControlSystem::UpdateControlParm()
{
  if(RemoteListener::GetData().rc.s2 != 2)
  {
    /*TODO 此处为遥控器控制*/
    HolderControlUnit::UpdateControlParmForRemote();
    ShootControlUnit::UpdateControlParmForRemote();

  }
  else
  {
    /*TODO 此处为PC控制*/
    HolderControlUnit::UpdateControlParmForPC();
    ShootControlUnit::UpdateControlParmForPC();
  }

}

bool MotorControlSystem::HolderOnline()
{
  return HolderControlUnit::IsOnline ();
}

void MotorControlSystem::RunHolder ()
{
  HolderControlUnit::RunHolderForControlParm();
}

bool MotorControlSystem::ShootOnline ()
{
  return ShootControlUnit::IsOnline();
}

void MotorControlSystem::RunShoot ()
{
  ShootControlUnit::RunShootForControlParm();
}

void HolderController(void const * argument)
{
  for(;;)
  {
    if(MotorControlSystem::HolderOnline())
    {
      MotorControlSystem::RunHolder();
    }
    osDelay(1);
  }
}
void ShootController(void const * argument)
{
  for(;;)
  {
    if(MotorControlSystem::ShootOnline())
    {
      MotorControlSystem::RunShoot();
    }
    osDelay(5);
  }
}