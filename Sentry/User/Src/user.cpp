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
#include <stdio.h>
#include <usart.h>

void user_main() {
  System::pc::setHandler(&huart2);
  printf("Sentry Init...\r\n");
  CanBusManger::Init();
  RemoteListener::Init();
  AttitudeListener::Init();
  CourtSystem::Init();
  MiniPC::Init();
  AutoMA::Init();
  MotorControlSystem::Init();

  printf("Sentry Init ok");
}

void StartDefaultTask(void const *argument) {
//  System::pc::println("Hello Jelin");
  Port redLine(GPIOG, GPIO_PIN_13);
  redLine.set();

  for (;;) {
    if(!MiniPC::isOnline())
    {
      MiniPC::Start();
    }
    osDelay(1000);
  }
}
