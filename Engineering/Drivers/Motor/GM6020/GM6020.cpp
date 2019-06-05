//
// Created by Administrator on 2019/4/7.
//

#include "GM6020.h"
#include <AttitudeListener.h>

void GM6020::UpdateParm()
{
  C6020::UpdateParm();
  u8* data = canListener->GetData();
  mAngleData = ((u16)data[0]<<8) + data[1];
  mTemperature = data[6];

}


void GM6020::printParm()
{
  System::pc::println("[Motor:0x%02x]:%5d, %5d,%5d",mPort,mTorque,mSpeed,(int)mAngle);
}

GM6020::GM6020(u16 port1, PID *pid, PID *_pidAngle) : C6020(port1, pid)
{
  C6020(port1, pid);
  mPIDAngel = _pidAngle;
}

float GM6020::AnglePIDOut(float angle)
{
  mPIDAngel->setTarget(angle);
  //System::pc::println("mAngle:%d",(int)mAngle);
  return mPIDAngel->calculate(mAngle);
}


u16 GM6020::GetAngleData()
{
  return mAngleData;
}

short GM6020::SpeedPIDOut(float target, float current)
{
  mPID->setTarget(target);
  return mPID->calculate(current);
}

void GM6020::SetAngle(float angle)
{
  mAngle = angle;
}



