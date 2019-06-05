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
  Port power1(GPIOH,GPIO_PIN_2);
  Port power2(GPIOH,GPIO_PIN_3);
  Port power3(GPIOH,GPIO_PIN_4);
  Port power4(GPIOH,GPIO_PIN_5);

  power1.set();
  power2.set();
  power3.set();
  power4.set();

  for (;;) {
    osDelay(100);
  }
}
