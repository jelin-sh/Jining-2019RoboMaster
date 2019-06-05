//
// Created by Administrator on 2019/4/6.
//

#include "ChassisControlUnit.h"
#include "Plus_List.h"
#include <C6020/C6020.h>
#include <CanBusManger.h>
#include <CourtSystem.h>
#include <RemoteListener.h>
#include <cmath>

Plus_List<C6020 *> *motorList;
Port *switchChassisLeft;
Port *switchChassisRight;

u16 chassisMoveData = 0;
bool autoMoveChassis = true;
SpeedStruct speedStruct;
u8 powerMax = 20; //限定最大功率为80W
u16 chassisSpeedMax = CHASSIS_SPEED_MAX;

#define CHASSIS_SPEED_BASE (660.0 / chassisSpeedMax) //底盘增量速度

void ChassisControlUnit::Init() {
  switchChassisRight = new Port(GPIOA, GPIO_PIN_2);
  switchChassisLeft = new Port(GPIOA, GPIO_PIN_3);

  motorList = new Plus_List<C6020 *>();
  motorList->add(
      new C6020(CAN_ID_CHASSIS1, new PID(10000, 1000, 0, 0, 2, 0.06, 2.5)));
  motorList->add(
      new C6020(CAN_ID_CHASSIS2, new PID(10000, 1000, 0, 0, 2, 0.06, 2.5)));
  srand(6666);
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
  for (int i = 0; i < 2; i++) {
    motorList->get(i)->UpdateParm();
  }
}

void ChassisControlUnit::UpdateControlParmForRemote() {
//  speedStruct.forward_back_ch = static_cast<int>(
//      (RemoteListener::GetData().rc.ch1 - REMOTE_MID_VAL) / CHASSIS_SPEED_BASE);
//  speedStruct.left_right_ch = static_cast<int>(
//      (RemoteListener::GetData().rc.ch0 - REMOTE_MID_VAL) / CHASSIS_SPEED_BASE);
//  speedStruct.rotate_ch = static_cast<int>(
//      (RemoteListener::GetData().rc.ch2 - REMOTE_MID_VAL) / CHASSIS_SPEED_BASE);
//
//  //autoMoveChassis = RemoteListener::GetData().rc.s1 == 2;

  if (switchChassisRight->get() == Port::Port_Status_Off) {
    chassisMoveData = 0;
  }
  if (switchChassisLeft->get() == Port::Port_Status_Off) {
    chassisMoveData = 0;
  }
}
void ChassisControlUnit::RunMotorForControlParm() {
  short outVal[4] = {0};
  float I = 0, U = 0;
  float power = 0;
  short inVal[2] = {0};
  static short tem = -1; //-1:lift 1:right


  if (autoMoveChassis) {
    if (chassisMoveData == 0) {
      do {
        chassisMoveData = rand() % 1000;
      } while (chassisMoveData < 500);

      if (switchChassisRight->get() == Port::Port_Status_Off) {
        tem = -1;
      } else if(switchChassisLeft->get() == Port::Port_Status_Off) {
        tem = 1;
      } else{
        tem = rand()%2;
        if(!tem)
        {
          tem--;
        }
      }

    } else {
      chassisMoveData--;
      inVal[0] = tem * CHASSIS_SPEED_MAX;
      inVal[1] = tem * CHASSIS_SPEED_MAX;
    }
  } else {
    inVal[0] = speedStruct.left_right_ch;
    inVal[1] = speedStruct.left_right_ch;
  }

  // inVal 对应每个电机的速度
  // outVal 对应每个电机的输出值

  //滤波->计算输出
  for (int i = 0; i < 2; i++) {
    if (fabs(inVal[i]) > CHASSIS_SPEED_MAX) {
      inVal[i] = fabs(inVal[i]) / inVal[i] * CHASSIS_SPEED_MAX;
    }

    outVal[i] = motorList->get(i)->CalcOutput(inVal[i]);
    //
  }
  //限制输出功率
  power = CourtSystem::GetPowerHeatData()->chassis_power;
  // System::pc::printf("Power:%d\r\n",(u8)power);
  // System::pc::println("Power:%d.%d",(u16)power,(u16)((power-(u16)power)*100));
  if (power >= powerMax) {
    /*TODO 循环计算9次*/
    chassisSpeedMax -= (power-powerMax);

    if (chassisSpeedMax < 2000) {
      chassisSpeedMax = 2000;
    }
    for (int j = 0; j < 2; j++) {
      inVal[j] = inVal[j] * 0.85;
      outVal[j] = motorList->get(j)->CalcOutput(inVal[j]);

      if (fabs(outVal[j]) < 1000) {
        outVal[j] = fabs(outVal[j]) / outVal[j] * 1000;
      }
    }

  } else {
    if (power > 20 && power < 50) {
      chassisSpeedMax += 2;
      if (chassisSpeedMax > 3000) {
        chassisSpeedMax = 3000;
      }
    } else if(power < 20 ){
      chassisSpeedMax--;
      if (chassisSpeedMax < 2000) {
        chassisSpeedMax = 2000;
      }
    }
    for (int i = 0; i < 2; i++) {
      float tem = motorList->get(i)->GetMaxOut() * 1.15;
      if (tem > 10000) {
        tem = 10000;
      }
      motorList->get(i)->SetMaxOut(tem);
    }
  }

  //上传
  for (int i = 0; i < 2; i++) {
    motorList->get(i)->GetListener()->WriteData(outVal[i]);
  }

  // System::pc::println("targetSpeed:%d,speed:%d",inVal[0],motorList->get(0)->GetSpeed());
}

bool ChassisControlUnit::IsOnline() {
  return (motorList->get(0)->GetListener()->isOnline() &&
          motorList->get(1)->GetListener()->isOnline());
}
bool ChassisControlUnit::IsRevolve() { return speedStruct.rotate_ch != 0; }

int ChassisControlUnit::GetRevolveSpeed() { return speedStruct.rotate_ch; }
