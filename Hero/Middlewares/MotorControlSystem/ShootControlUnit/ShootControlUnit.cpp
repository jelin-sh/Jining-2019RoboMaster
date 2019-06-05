//
// Created by Administrator on 2019/4/30.
//

#include "ShootControlUnit.h"
#include <C6010/C6010.h>
#include <CanBusManger.h>
#include <RemoteListener.h>
#include <pwm/pwm.h>
#include <tim.h>

PWM *shootMotor_s1;
PWM *shootMotor_s2;
PWM *shootMotor_b1;
PWM *shootMotor_b2;
PWM *shootSwitch;
C6010 *pordMotor1;
C6010 *pordMotor2;
u32 shootSpeed1 = 2000;
u32 shootSpeed2 = 1280;

bool bigShotLock = false;
bool shootControl1 = false;
bool shootControl2 = false;

void ShootControlUnit::Init() {
  shootMotor_s1 = new PWM(&htim8, TIM_CHANNEL_1);
  shootMotor_s2 = new PWM(&htim8, TIM_CHANNEL_2);
  shootMotor_b1 = new PWM(&htim8, TIM_CHANNEL_3);
  shootMotor_b2 = new PWM(&htim8, TIM_CHANNEL_4);
  shootSwitch = new PWM(&htim5, TIM_CHANNEL_1);
  pordMotor1 =
      new C6010(CAN_ID_SHOOT1, new PID(10000, 1000, 0, 0, 1.5, 0.001, 2));
  pordMotor2 =
      new C6010(CAN_ID_SHOOT2, new PID(10000, 1000, 0, 0, 3, 0.002, 2));
  shootSwitch->out(1500);
  shootSwitch->start();
}

void ShootControlUnit::OpenShootMotor() {
  HAL_Delay(5000);
  shootMotor_s1->out(800);
  shootMotor_s2->out(800);
  shootMotor_b1->out(800);
  shootMotor_b2->out(800);
  shootMotor_s1->start();
  shootMotor_s2->start();
  shootMotor_b1->start();
  shootMotor_b2->start();
  HAL_Delay(2000);
  for (int k = 800; k < shootSpeed1; k++) {
    shootMotor_s1->out(k);
    shootMotor_s2->out(k);
  }
  for (int k = 800; k < shootSpeed2; k++) {
    shootMotor_b1->out(k);
    shootMotor_b2->out(k);
  }
}

void ShootControlUnit::OnSmall() {
  pordMotor1->GetListener()->WriteData(pordMotor1->CalcOutput(2000));
}

void ShootControlUnit::OffSmall() { pordMotor1->GetListener()->WriteData(0); }

void ShootControlUnit::UpdateCanParm() {
  pordMotor1->UpdateParm();
  pordMotor2->UpdateParm();
}

void ShootControlUnit::UpdateControlParmForRemote() {
  shootControl1 = (RemoteListener::GetData().rc.s1 == 1);
  shootControl2 = (RemoteListener::GetData().rc.s1 == 2);
}

bool ShootControlUnit::IsOnline() {
  return pordMotor2->GetListener()->isOnline();
}

void ShootControlUnit::RunShootForControlParm() {
  static u16 lockTime = 0;
  static u16 runTime = 0;

  if (shootControl1) {
    OnBig();
  } else {
    OffBig();
  }

  if (shootControl2) {
    if (!bigShotLock) {
      runTime++;
      shootSwitch->out(1600);
      if (runTime > 69) {
        runTime = 0;
        bigShotLock = true;
      }
    } else {
      lockTime++;
      shootSwitch->out(1500);
      if (lockTime > 1000) {
        lockTime = 0;
        bigShotLock = false;
      }
    }

  } else {
    lockTime++;
    shootSwitch->out(1500);
    if (lockTime > 600) {
      lockTime = 0;
      bigShotLock = false;
    }

  }
}

void ShootControlUnit::UpdateControlParmForPC() {
  shootControl2 = (RemoteListener::GetData().mouse.press_l == 1);
  shootControl1 = RemoteListener::HaveKey(KEY::KEY_E);
}
void ShootControlUnit::OnBig() {
  pordMotor2->GetListener()->WriteData(pordMotor2->CalcOutput(1000));
}
void ShootControlUnit::OffBig() { pordMotor2->GetListener()->WriteData(0); }
