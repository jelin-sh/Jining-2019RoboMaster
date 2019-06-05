//
// Created by jelin on 19-5-12.
//

extern "C" {
#include <cmsis_os.h>
}

#include <AttitudeListener.h>
#include <CanBusManger.h>
#include <CourtSystem.h>
#include <MotorControlSystem.h>
#include <MyThread.h>
#include <RemoteListener.h>
#include <system/System.h>
#include <usart.h>
#include <user.h>

void user_main() {
  System::pc::setHandler(&huart2);
  System::pc::println("Hero Init...");
  CanBusManger::Init();
  RemoteListener::Init();
  AttitudeListener::Init();
  CourtSystem::Init();
  MotorControlSystem::Init();
  System::pc::println("Hero Init Ok!");
}

void StartDefaultTask(void const *argument) {

  Port redLine(GPIOG, GPIO_PIN_13);
  redLine.set();
  for (;;) {
    osDelay(100);
  }
}
