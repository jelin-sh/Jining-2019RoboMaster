//
// Created by jelin on 19-5-13.
//

#ifndef HERO_MYTHREAD_H
#define HERO_MYTHREAD_H

#include <cmsis_os.h>
#include <stm32plus.h>
class MyThread {
private:
  osThreadId id;
  osThreadDef_t attr;
public:
  MyThread(char* name,
      u32 stake,
      osPriority priority,
      os_pthread function,
      void* arg);
  void Run();
  osThreadId GetId();
};



#endif //HERO_MYTHREAD_H
