//
// Created by Administrator on 2019/4/5.
//

#include "user.h"
#include <AttitudeListener.h>
#include <AutoMA.h>
#include <C6020.h>
#include <CanBusManger.h>
#include <CourtSystem.h>
#include <MiniPC.h>
#include <MotorControlSystem.h>
#include <RemoteListener.h>
#include <cmsis_os.h>
#include <usart.h>

void user_main() {
  System::pc::setHandler(&huart2);
  System::pc::println("Infentery init...");
  CanBusManger::Init();
  RemoteListener::Init();
  AttitudeListener::Init();
  CourtSystem::Init();
  MotorControlSystem::Init();

  System::pc::println("Init OK");
}

void StartDefaultTask(void const *argument) {
  System::pc::println("Hello Jelin");
  Port redLine(GPIOG, GPIO_PIN_13);
  redLine.set();

  for (;;) {
    osDelay(1);
  }
}
