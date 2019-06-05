//
// Created by jelin on 19-5-17.
//

#include "AutoMA.h"
#include <MiniPC.h>
#include <PID.h>
PID* pidPitchMA;
PID* pidYawMA;

float AutoMA::CalcPithAngle() {
  u16 y = MiniPC::GetY();
  if(y == 0)
  {
    return 0;
  }
  float res = 0;
  res = pidPitchMA->calculate(y);
  MiniPC::ClearY();
  return res;
}
float AutoMA::CalcYawAngle() {
  u16 x = MiniPC::GetX();
  //System::pc::println("x:%d",x);
  if(x == 0)
  {
    return 0;
  }
  float res = 0;
  //short err = x - AUTOMA_YAW_MID;
  res = pidYawMA->calculate(x);
  //System::pc::println("res:%d",(int)(res*100));
  MiniPC::ClearX();
  return res;
}
void AutoMA::Init() {
  pidPitchMA = new PID(100,0,0,AUTOMA_PITCH_MID+40,0.02,0,0);
  pidYawMA = new PID(100,20,0,AUTOMA_YAW_MID,0.02,0.001,0);
}
