//
// Created by Administrator on 2019/4/7.
//

#include "HolderControlUnit.h"
#include <AttitudeListener.h>
#include <AutoMA.h>
#include <CanBusManger.h>
#include <ChassisControlUnit/ChassisControlUnit.h>
#include <GM6020/GM6020.h>
#include <MiniPC.h>
#include <RemoteListener.h>
#include <cmath>
#include <stdio.h>

GM6020 *motorYaw;
GM6020 *motorPitch;
Kalman *kalmanYaw;
Kalman *kalmanPitch;
Kalman *kalmanTargetYaw;
Kalman *kalmanTargetPitch;

float targetYawAngle = HOLDER_YAW_ANGLE_MID;
float targetPitchAngle = HOLDER_PITCH_ANGLE_MID;

bool lockYaw = false;
bool autoMASign = false;
bool autoRoaud = false;
void HolderControlUnit::Init() {
  /*
   * new PID(20000, 10000 , 0, 0, 2000,10,0)
   * new PID(50, 1, 0, 0, 0.5,0,0.0003)
   * */
  motorYaw =
      new GM6020(CAN_ID_HODERYAW,
                 new PID(20000, 20000, 0, 0, 3000, 110, 500) /*速度环PID*/,
                 new PID(100, 0, 0, 0, 0.07, 0, 0) /*位置环PID*/);

  motorPitch =
      new GM6020(CAN_ID_HODERPITCH, new PID(20000, 20000, 0, 0, 3000, 110, 500),
                 new PID(100, 0, 0, 0, 0.07, 0, 0));
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
void HolderControlUnit::RunYaw(float angle) {
  /*TODO 目标位置大于极限位置 则等于极限位置*/
  if (angle > HOLDER_YAW_ANGLE_MAX) {
    angle = HOLDER_YAW_ANGLE_MAX;
  }
  if (angle < HOLDER_YAW_ANGLE_MIN) {
    angle = HOLDER_YAW_ANGLE_MIN;
  }

  /*计算位置环PID 输出目标速度*/
  float targetSpeed = 0;
  float currentSpeed = 0;
  float currentAngle = 0;
  if (!AttitudeListener::IsBusy()) {
    currentSpeed = AttitudeListener::getImu()->wz;
  } else {
    //陀螺仪数据获取失败
    return;
  }
  currentAngle = motorYaw->GetAngel();
  if (currentAngle <= 0) {
    currentAngle += 360;
    motorYaw->SetAngle(currentAngle);
  }
  if (autoRoaud) {
    targetSpeed = 3;
  }

  /*TODO  如果当前位置等于或超过极限位置,不允许继续前进*/
  short targetCurrent = motorYaw->SpeedPIDOut(targetSpeed, currentSpeed);
  if (motorYaw->GetAngel() >= HOLDER_YAW_ANGLE_MAX) {
    if (targetCurrent > 0) {
      targetCurrent = 0;
    }
  } else if (motorYaw->GetAngel() <= HOLDER_YAW_ANGLE_MIN) {
    if (targetCurrent < 0) {
      targetCurrent = 0;
    }
  }
  motorYaw->GetListener()->WriteData(targetCurrent);
}

bool HolderControlUnit::IsOnline() {
  return (motorPitch->GetListener()->isOnline() &&
          motorYaw->GetListener()->isOnline());
}

void HolderControlUnit::UpdateCanParm() {
  static u32 listTime = 0;
  u32 thisTime = HAL_GetTick();
  float dt = thisTime - listTime;
  listTime = thisTime;
  dt /= 1000;
  motorYaw->UpdateParm();
  motorPitch->UpdateParm();
  if (!AttitudeListener::IsBusy()) {
    float yaw = kalmanYaw->getAngle(motorYaw->GetAngel(),
                                    AttitudeListener::getImu()->wz, dt);
    float pitch = kalmanPitch->getAngle(motorPitch->GetAngel(),
                                        AttitudeListener::getImu()->wx, dt);
    if (yaw <= 0) {
      yaw += 360;
    }
    if (pitch <= 0) {
      pitch += 360;
    }
    motorYaw->SetAngle(yaw);
    motorPitch->SetAngle(pitch);
    printf("%d,%d\r\n", (int)yaw, (int)pitch);//180 196
    // System::pc::println("temp:%d",motorPitch->GetTorque());
  }

  // motorYaw->printParm();
}

void HolderControlUnit::RunPitch(float angle) {
  /*TODO 目标位置大于极限位置 则等于极限位置*/
  if (angle > HOLDER_PITCH_ANGLE_MAX) {
    angle = HOLDER_PITCH_ANGLE_MAX;
  }
  if (angle < HOLDER_PITCH_ANGLE_MIN) {
    angle = HOLDER_PITCH_ANGLE_MIN;
  }

  float targetSpeed = motorPitch->AnglePIDOut(angle);
  float currentSpeed = 0;
  if (!AttitudeListener::IsBusy()) {
    currentSpeed = AttitudeListener::getImu()->wx;
  } else {
    return;
  }

  /*TODO  如果当前位置等于或超过极限位置,不允许继续前进*/
  short targetCurrent = motorPitch->SpeedPIDOut(targetSpeed, currentSpeed);

  if (motorPitch->GetAngel() >= HOLDER_PITCH_ANGLE_MAX) {
    if (targetCurrent > 0) {
      targetCurrent = 0;
    }
  } else if (motorPitch->GetAngel() <= HOLDER_PITCH_ANGLE_MIN) {
    if (targetCurrent < 0) {
      targetCurrent = 0;
    }
  }
  motorPitch->GetListener()->WriteData(targetCurrent);
}

void HolderControlUnit::RunHolderForControlParm() {
//  if (autoMASign) {
//    if (MiniPC::GetX() == 0 && MiniPC::GetY() == 0) {
//      autoRoaud = true;
//    } else {
//      autoRoaud = false;
//      targetYawAngle += AutoMA::CalcYawAngle();
//      targetPitchAngle -= AutoMA::CalcPithAngle();
//    }
//  } else{
//    autoRoaud = false;
//  }
  RunYaw(targetYawAngle);
  RunPitch(targetPitchAngle);
}

void HolderControlUnit::UpdateControlParmForRemote() {
  //autoMASign = (RemoteListener::GetData().rc.s2 == 1);
}
