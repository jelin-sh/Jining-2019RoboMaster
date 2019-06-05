//
// Created by Administrator on 2019/4/6.
//

#include <CanBusManger.h>
#include "C6020.h"

C6020::C6020(u16 port, PID *pid)
{
  mPort = port;
  mPID = pid;
  canListener = new CanListener(mPort);
}

void C6020::UpdateParm()
{
  u8* data = canListener->GetData();
  mAngle = (((u16)data[0]<<8) + data[1])/22.757777f;
  mSpeed = ((u16)data[2]<<8) +  data[3];
  mTorque = ((u16)data[4]<<8) + data[5];
}

void C6020::printParm()
{
  System::pc::println("[Motor:0x%02x]:%5d, %5d",mPort,mTorque,mSpeed);
}

short C6020::GetSpeed()
{
  return mSpeed;
}

short C6020::GetTorque()
{
  return mTorque;
}

float C6020::GetAngel()
{
  return mAngle;
}

short C6020::CalcOutput(short targetSpeed)
{
  mPID->setTarget(targetSpeed);
  //System::pc::printf("motor 0x%02x target:%d,measure:%d\r\n",mPort,targetSpeed,mSpeed);
  return mPID->calculate(mSpeed);
}

CanListener *C6020::GetListener()
{
  return canListener;
}

void C6020::SetMaxOut(float maxOut) {

    mPID->setOutMax(maxOut);


}
float C6020::GetMaxOut() { return  mPID->getOutMax(); }
