//
// Created by Administrator on 2019/4/30.
//

#include <pwm/pwm.h>
#include <tim.h>
#include <C6010/C6010.h>
#include <CanBusManger.h>
#include <RemoteListener.h>
#include "ShootControlUnit.h"

PWM* shootMotor1;
PWM* shootMotor2;
C6010* pordMotor;
u32 shootSpeed = 120;
bool shootControl = false;
void ShootControlUnit::Init ()
{
  shootMotor1 = new PWM(&htim8,TIM_CHANNEL_1);
  shootMotor2 = new PWM(&htim8,TIM_CHANNEL_2);
  pordMotor = new C6010(CAN_ID_SHOOT,new PID(10000,1000,0,0,1.5,0.001,2));
}


void ShootControlUnit::OpenShootMotor ()
{
  HAL_Delay(8000);
  shootMotor1->out(250);
  shootMotor2->out(250);
  shootMotor1->start();
  shootMotor2->start();
  HAL_Delay(2000);
  shootMotor1->out(100);
  shootMotor2->out(100);
  HAL_Delay(4000);
  for(int k = 100;k<shootSpeed;k++)
  {
    HAL_Delay(10);
  }
}

void ShootControlUnit::SetShootMotorSpeed (ShootControlUnit::ShootMotorSpeed speed)
{
  switch((int)speed)
  {
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

void ShootControlUnit::On ()
{
  pordMotor->GetListener()->WriteData(pordMotor->CalcOutput(4000));
}

void ShootControlUnit::Off ()
{
  pordMotor->GetListener()->WriteData(0);
}

void ShootControlUnit::UpdateCanParm ()
{
  pordMotor->UpdateParm();
//  if(pordMotor->GetSpeed()>10)
//  {
//    System::pc::println("PortMotorSpeed:%d",pordMotor->GetSpeed());
//  }

}

void ShootControlUnit::UpdateControlParmForRemote ()
{
  shootControl = (RemoteListener::GetData().rc.s1 == 1);
  if(RemoteListener::GetData().rc.s2 == 1)
  {
    shootMotor1->out(shootSpeed);
    shootMotor2->out(shootSpeed);
  }
  else{
    shootMotor1->out(100);
    shootMotor2->out(100);
  }
}

bool ShootControlUnit::IsOnline ()
{
  return pordMotor->GetListener()->isOnline();
}

void ShootControlUnit::RunShootForControlParm ()
{
  if(shootControl)
  {
    On();
  }
  else
  {
    Off();
  }
}

void ShootControlUnit::UpdateControlParmForPC() {

  shootControl = RemoteListener::GetData().mouse.press_l == 1;

}

