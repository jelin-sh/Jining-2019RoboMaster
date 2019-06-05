//
// Created by Administrator on 2019/4/6.
//

#include "ChassisControlUnit.h"
#include "Plus_List.h"
#include <C6020/C6020.h>
#include <CanBusManger.h>
#include <CourtSystem.h>
#include <RemoteListener.h>
#include <TakeShotUnit/TakeShotUnit.h>
#include <cmath>

Plus_List<C6020 *> *motorList;
SpeedStruct speedStruct = {0};
u8 powerMax = 80; //限定最大功率为80W

void ChassisControlUnit::Init() {
  motorList = new Plus_List<C6020 *>();
  motorList->add(
      new C6020(CAN_ID_CHASSIS1, new PID(10000, 1000, 0, 0, 2, 0.06, 2.5)));
  motorList->add(
      new C6020(CAN_ID_CHASSIS2, new PID(10000, 1000, 0, 0, 2, 0.06, 2.5)));
  motorList->add(
      new C6020(CAN_ID_CHASSIS3, new PID(10000, 1000, 0, 0, 2, 0.06, 2.5)));
  motorList->add(
      new C6020(CAN_ID_CHASSIS4, new PID(10000, 1000, 0, 0, 2, 0.06, 2.5)));
}

void ChassisControlUnit::RunMotor(u16 port, short speed) {
  C6020 *motor = motorList->get(port - CAN_ID_CHASSIS1);
  if (motor == nullptr) {
    //防止误调用
    return;
  }
  short outVal = motor->CalcOutput(speed);
  motor->GetListener()->WriteData(outVal); //对Motor的CanListener上传数据
  // System::pc::printf("outval:%d\r\n",outVal);
}

void ChassisControlUnit::UpdateCanParm() {
  /**更新电机参数**/
  for (int i = 0; i < 4; i++) {
    motorList->get(i)->UpdateParm();
  }
}

void ChassisControlUnit::UpdateControlParmForRemote() {
  speedStruct.forward_back_ch = static_cast<int>(
      (RemoteListener::GetData().rc.ch1 - REMOTE_MID_VAL) / CHASSIS_SPEED_BASE);
  speedStruct.left_right_ch = static_cast<int>(
      (RemoteListener::GetData().rc.ch0 - REMOTE_MID_VAL) / CHASSIS_SPEED_BASE);
  speedStruct.rotate_ch = static_cast<int>(
      (RemoteListener::GetData().rc.ch2 - REMOTE_MID_VAL) / CHASSIS_SPEED_BASE);
}

void ChassisControlUnit::RunMotorForControlParm() {
  short outVal[4] = {0};
  float I = 0, U = 0;
  short inVal[4] = {0};
  // inVal 对应
  //
  // 每个电机的速度
  inVal[0] = -speedStruct.forward_back_ch + speedStruct.left_right_ch +
             speedStruct.rotate_ch;
  inVal[1] = -speedStruct.forward_back_ch - speedStruct.left_right_ch +
             speedStruct.rotate_ch;
  inVal[2] = speedStruct.forward_back_ch + speedStruct.left_right_ch +
             speedStruct.rotate_ch;
  inVal[3] = speedStruct.forward_back_ch - speedStruct.left_right_ch +
             speedStruct.rotate_ch;
  // outVal 对应每个电机的输出值
  if (TakeShotUnit::IsTakeMode()) {
    inVal[0] = -inVal[0];
    inVal[1] = -inVal[1];
    inVal[2] = -inVal[2];
    inVal[3] = -inVal[3];
  }
  //滤波->计算输出
  for (int i = 0; i < 4; i++) {
    if (fabs(inVal[i]) > CHASSIS_SPEED_MAX) {
      inVal[i] = fabs(inVal[i]) / inVal[i] * CHASSIS_SPEED_MAX;
    }

    outVal[i] = motorList->get(i)->CalcOutput(inVal[i]);
    //
  }
  //限制输出功率
  // System::pc::println("Power:%d",(u8)CourtSystem::GetPowerHeatData()->chassis_power);
//  if (CourtSystem::GetPowerHeatData()->chassis_power >= powerMax) {
//    /*TODO 循环计算9次*/
//    for (int i = 9; i > 0; i--) {
//      I = 0;
//      U = CourtSystem::GetPowerHeatData()->chassis_volt;
//      for (int j = 0; j < 4; j++) {
//        inVal[j] = (short)(inVal[j] * i / 9.5);
//        motorList->get(j)->SetMaxOut(10000 * (i / 10.0));
//        outVal[j] = motorList->get(j)->CalcOutput(inVal[j]);
//        I += fabs(outVal[j]) / 819.2;
//      }
//      if ((U * I) < powerMax) {
//        break;
//      }
//    }
//  } else {
//
    for (int i = 0; i < 4; i++) {
      motorList->get(i)->SetMaxOut(10000);
    }
//  }

  //上传
  for (int i = 0; i < 4; i++) {
    motorList->get(i)->GetListener()->WriteData(outVal[i]);
  }

  // System::pc::println("targetSpeed:%d,speed:%d",inVal[0],motorList->get(0)->GetSpeed());
}

bool ChassisControlUnit::IsOnline() {
  return (motorList->get(0)->GetListener()->isOnline() &&
          motorList->get(1)->GetListener()->isOnline() &&
          motorList->get(2)->GetListener()->isOnline() &&
          motorList->get(3)->GetListener()->isOnline());
}
bool ChassisControlUnit::IsRevolve() { return speedStruct.rotate_ch != 0; }

void ChassisControlUnit::UpdateControlParmForPC() {
  static short keyCtrEnterCount = 0;

  static short keyAEnterCount = 0;
  static short keySEnterCount = 0;
  static short keyDEnterCount = 0;
  static short keyWEnterCount = 0;

  static int left_right_ch = 0;
  static short left_start_count = 0;
  static int forward_back_ch = 0;
  static short forward_start_count = 0;

  static u16 roundCount = 0;
  static short targetRotate = 0;
  //冲突键
  static KEY keyConflicts[2][2] = {{KEY::KEY_A, KEY::KEY_D},
                                   {KEY::KEY_W, KEY::KEY_S}};

  roundCount = 0;
  keyCtrEnterCount = 0;
  /*Todo MouseControl*/
  if (!RemoteListener::HaveKey(KEY::KEY_Q)) {
    targetRotate = RemoteListener::GetData().mouse.x * 300;
    if (TakeShotUnit::IsTakeMode()) {
      targetRotate *= -1;
    }
    if (speedStruct.rotate_ch > targetRotate) {
      speedStruct.rotate_ch -= 200;
    } else if (speedStruct.rotate_ch < targetRotate) {
      speedStruct.rotate_ch += 200;
    }
  }

  /*检测键位是否冲突,此处已经优化
  for(int i = 0;i<2;i++)
  {
    if(RemoteListener::HaveKey(keyConflicts[i][0]) &&
  RemoteListener::HaveKey(keyConflicts[i][1]))
    {
      return;
    }
  }
  */
  /*TODO 无冲突,执行键盘控制*/
  //-----------------------------------------------------------------左移
  if (RemoteListener::HaveKey(KEY::KEY_A) && keyDEnterCount == 0) {
    if (keyAEnterCount < REMOTE_THREAD_DELAY(1000)) {
      //斜坡启动时间是1s
      keyAEnterCount++;
    }
    speedStruct.left_right_ch =
        keyAEnterCount * -(CHASSIS_SPEED_MAX / REMOTE_THREAD_DELAY(1000));
    left_right_ch = speedStruct.left_right_ch;
    left_start_count = keyAEnterCount;
  } else if (keyDEnterCount == 0) {
    // 0.1s的斜坡制动
    if (keyAEnterCount > 0) {
      keyAEnterCount -= left_start_count * 0.05;
      if (keyAEnterCount < 0) {
        keyAEnterCount = 0;
      }
    }
    speedStruct.left_right_ch =
        keyAEnterCount * (left_right_ch / left_start_count);
  }
  //-----------------------------------------------------------------右移
  if (RemoteListener::HaveKey(KEY::KEY_D) && keyAEnterCount == 0) {
    if (keyDEnterCount < REMOTE_THREAD_DELAY(1000)) {
      //斜坡启动时间是1s
      keyDEnterCount++;
    }
    speedStruct.left_right_ch =
        keyDEnterCount * (CHASSIS_SPEED_MAX / REMOTE_THREAD_DELAY(1000));
    left_right_ch = speedStruct.left_right_ch;
    left_start_count = keyDEnterCount;
  } else if (keyAEnterCount == 0) {
    // 0.1s的斜坡制动
    if (keyDEnterCount > 0) {
      keyDEnterCount -= left_start_count * 0.04;
      if (keyDEnterCount < 0) {
        keyDEnterCount = 0;
      }
    }
    speedStruct.left_right_ch =
        keyDEnterCount * (left_right_ch / left_start_count);
  }
  //-----------------------------------------------------------------向前
  if (RemoteListener::HaveKey(KEY::KEY_W) && keySEnterCount == 0) {
    if (keyWEnterCount < REMOTE_THREAD_DELAY(1000)) {
      //斜坡启动时间是1s
      keyWEnterCount++;
    }
    speedStruct.forward_back_ch =
        keyWEnterCount * (CHASSIS_SPEED_MAX / REMOTE_THREAD_DELAY(1000));
    forward_back_ch = speedStruct.forward_back_ch;
    forward_start_count = keyWEnterCount;
  } else if (keySEnterCount == 0) {
    // 0.1s的斜坡制动
    if (keyWEnterCount > 0) {
      keyWEnterCount -= forward_start_count * 0.04;
      if (keyWEnterCount < 0) {
        keyWEnterCount = 0;
      }
    }
    speedStruct.forward_back_ch =
        keyWEnterCount * (forward_back_ch / forward_start_count);
  }
  //-----------------------------------------------------------------向后
  if (RemoteListener::HaveKey(KEY::KEY_S) && keyWEnterCount == 0) {
    if (keySEnterCount < REMOTE_THREAD_DELAY(1000)) {
      //斜坡启动时间是1s
      keySEnterCount++;
    }
    speedStruct.forward_back_ch =
        keySEnterCount * -(CHASSIS_SPEED_MAX / REMOTE_THREAD_DELAY(1000));
    forward_back_ch = speedStruct.forward_back_ch;
    forward_start_count = keySEnterCount;
  } else if (keyWEnterCount == 0) {
    // 0.1s的斜坡制动
    if (keySEnterCount > 0) {
      keySEnterCount -= forward_start_count * 0.04;
      if (keySEnterCount < 0) {
        keySEnterCount = 0;
      }
    }
    speedStruct.forward_back_ch =
        keySEnterCount * (forward_back_ch / forward_start_count);
  }
}
