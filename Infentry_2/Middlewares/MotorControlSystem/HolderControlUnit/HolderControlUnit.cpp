//
// Created by Administrator on 2019/4/7.
//

#include "HolderControlUnit.h"
#include <AttitudeListener.h>
#include <AutoMA.h>
#include <CanBusManger.h>
#include <ChassisControlUnit/ChassisControlUnit.h>
#include <GM6020/GM6020.h>
#include <RemoteListener.h>
#include <cmath>

GM6020* motorYaw;
GM6020* motorPitch;
Kalman* kalmanYaw;
Kalman* kalmanPitch;
Kalman* kalmanTargetYaw;
Kalman* kalmanTargetPitch;

float targetYawAngle = HOLDER_YAW_ANGLE_MID;
float targetPitchAngle = HOLDER_PITCH_ANGLE_MID;

bool lockYaw = false;
bool autoMASign = false;
void HolderControlUnit::Init()
{
  /*
   * new PID(20000, 10000 , 0, 0, 2000,10,0)
   * new PID(50, 1, 0, 0, 0.5,0,0.0003)
   * */
  motorYaw = new GM6020(CAN_ID_HODERYAW,     new PID(20000, 20000 , 0, 0, 3500 ,110,500)/*速度环PID*/,
                                             new PID(100, 0, 0, 0, 0.07,0,0)/*位置环PID*/);

  motorPitch = new GM6020(CAN_ID_HODERPITCH, new PID(20000, 20000 , 0, 0, 2000,110,500),
                                             new PID(100, 0, 0, 0, 0.07,0,0));
  kalmanYaw = new Kalman();
  kalmanPitch = new Kalman();
  kalmanTargetYaw = new Kalman();
  kalmanTargetPitch = new Kalman();

  kalmanYaw->setQangle(0.1);

  kalmanTargetYaw->setQangle(0.1);
  kalmanTargetYaw->setAngle(HOLDER_YAW_ANGLE_MID);

  kalmanPitch->setQangle(0.03);
  kalmanTargetPitch->setQangle(0.03);
}
/*
 * @author:Jelin*/
void HolderControlUnit::RunYaw(float angle)
{
  /*TODO 目标位置大于极限位置 则等于极限位置*/
  if(angle > HOLDER_YAW_ANGLE_MAX)
  {
    angle = HOLDER_YAW_ANGLE_MAX;
  }
  if(angle < HOLDER_YAW_ANGLE_MIN )
  {
    angle = HOLDER_YAW_ANGLE_MIN;
  }

  /*计算位置环PID 输出目标速度*/
  float targetSpeed = 0;
  float currentSpeed = 0;
  float currentAngle = 0;
  if(!AttitudeListener::IsBusy())
  {
    currentSpeed = AttitudeListener::getImu()->wz;
  }
  else
  {
    //陀螺仪数据获取失败
    return;
  }
  currentAngle = motorYaw->GetAngel();
  if(currentAngle <= 0)
  {
    currentAngle += 360;
    motorYaw->SetAngle(currentAngle);
  }

  if(!lockYaw)
  {
    targetSpeed = motorYaw->AnglePIDOut(angle);
    float chassisSpeed = -ChassisControlUnit::GetRevolveSpeed()/19/60.0;
    if(ChassisControlUnit::IsRevolve())
    {
      currentSpeed -= chassisSpeed; /*TODO 与底盘陀螺仪做速度补偿*/
    }
  }

  /*TODO  如果当前位置等于或超过极限位置,不允许继续前进*/
  short targetCurrent = motorYaw->SpeedPIDOut(targetSpeed,currentSpeed);
  if(motorYaw->GetAngel() >= HOLDER_YAW_ANGLE_MAX)
  {
    if(targetCurrent > 0)
    {
      targetCurrent = 0;
    }
  }
  else if(motorYaw->GetAngel() <= HOLDER_YAW_ANGLE_MIN)
  {
    if(targetCurrent < 0)
    {
      targetCurrent = 0;
    }
  }
  motorYaw->GetListener()->WriteData(targetCurrent);
}


bool HolderControlUnit::IsOnline()
{
  return (motorPitch->GetListener()->isOnline() && motorYaw->GetListener()->isOnline());
}

void HolderControlUnit::UpdateCanParm()
{
  static u32 listTime = 0;
  u32 thisTime = HAL_GetTick();
  float dt = thisTime-listTime;
  listTime = thisTime;
  dt /= 1000;
  motorYaw->UpdateParm();
  motorPitch->UpdateParm();
  if(!AttitudeListener::IsBusy())
  {
    float yaw = kalmanYaw->getAngle(motorYaw->GetAngel(),AttitudeListener::getImu()->wz,dt);
    float pitch = kalmanPitch->getAngle(motorPitch->GetAngel(),AttitudeListener::getImu()->wx,dt);
    if(yaw <= 0)
    {
      yaw += 360;
    }
    if(pitch<=0)
    {
      pitch += 360;
    }
    motorYaw->SetAngle(yaw);
    motorPitch->SetAngle(pitch);
    //System::pc::println ("%d,%d",(int)yaw,(int)pitch);
    //System::pc::println("temp:%d",motorPitch->GetTorque());
  }


  //motorYaw->printParm();

}

void HolderControlUnit::RunPitch(float angle)
{
  /*TODO 目标位置大于极限位置 则等于极限位置*/
  if(angle > HOLDER_PITCH_ANGLE_MAX)
  {
    angle = HOLDER_PITCH_ANGLE_MAX;
  }
  if(angle < HOLDER_PITCH_ANGLE_MIN )
  {
    angle = HOLDER_PITCH_ANGLE_MIN;
  }

  float targetSpeed = motorPitch->AnglePIDOut(angle);
  float currentSpeed = 0;
  if(!AttitudeListener::IsBusy())
  {
    currentSpeed = AttitudeListener::getImu()->wx;
  }
  else
  {
    return;
  }

/*TODO  如果当前位置等于或超过极限位置,不允许继续前进*/
  short targetCurrent = motorPitch->SpeedPIDOut(targetSpeed,currentSpeed);

  if(motorPitch->GetAngel() >= HOLDER_PITCH_ANGLE_MAX)
  {
    if(targetCurrent > 0)
    {
      targetCurrent = 0;
    }
  }
  else if(motorPitch->GetAngel() <= HOLDER_PITCH_ANGLE_MIN)
  {
    if(targetCurrent < 0)
    {
      targetCurrent = 0;
    }
  }

//  if(RemoteListener::GetData().rc.s1 == 1)
//  {
//    targetSpeed = -1;
//  }
//  else if(RemoteListener::GetData().rc.s1 == 3)
//  {
//    targetSpeed = 0;
//  }
//  else if(RemoteListener::GetData().rc.s1 == 2)
//  {
//    targetSpeed = 1;
//  }


  motorPitch->GetListener()->WriteData(targetCurrent);

  //System::pc::printf ("target:%d,current:%d,out:%d\r\n",(int)angle,(int)motorPitch->GetAngel(),targetCurrent);
//  if(targetSpeed != 0)
//  {
//    System::pc::printf ("target:%d,current:%d,out:%d\r\n",(int)targetSpeed*100,(int)currentSpeed*100,targetCurrent);
//  }
}

void HolderControlUnit::UpdateControlParmForPC ()
{
  short x = RemoteListener::GetData().mouse.x;
  short y = RemoteListener::GetData().mouse.y;
  lockYaw = RemoteListener::HaveKey(KEY::KEY_Ctrl);

  //限定最大速度为6个的单位
  if(fabs(x)>6)
  {
    x = 6*(x/fabs(x));
  }
  if(fabs(y)>6)
  {
    y = 6*(y/fabs(y));
  }

  if(RemoteListener::HaveKey(KEY::KEY_Q))
  {
    if(!lockYaw)
    {
      targetYawAngle -= x * 0.5;
    }
  }
  else
  {
    targetYawAngle = HOLDER_YAW_ANGLE_MID;
  }

  targetPitchAngle += y*0.2;


  if(targetYawAngle > HOLDER_YAW_ANGLE_MAX)
  {
    targetYawAngle = HOLDER_YAW_ANGLE_MAX;
  }
  if(targetYawAngle < HOLDER_YAW_ANGLE_MIN )
  {
    targetYawAngle = HOLDER_YAW_ANGLE_MIN;
  }

  if(targetPitchAngle > HOLDER_PITCH_ANGLE_MAX)
  {
    targetPitchAngle = HOLDER_PITCH_ANGLE_MAX;
  }
  if(targetPitchAngle < HOLDER_PITCH_ANGLE_MIN )
  {
    targetPitchAngle = HOLDER_PITCH_ANGLE_MIN;
  }


}

void HolderControlUnit::RunHolderForControlParm ()
{
  if(autoMASign)
  {
    targetYawAngle+=AutoMA::CalcYawAngle();
    //targetYawAngle = kalmanTargetYaw->getAngle(targetYawAngle,0.1,1);
    targetPitchAngle-=AutoMA::CalcPithAngle();
  }
  HolderControlUnit::RunPitch(targetPitchAngle);
  //System::pc::println("targetYaw:%d",(int)targetYawAngle);
  HolderControlUnit::RunYaw(targetYawAngle);
}

void HolderControlUnit::UpdateControlParmForRemote ()
{
  lockYaw = (RemoteListener::GetData().rc.s2 == 1);
}
