//
// Created by jelin on 19-5-13.
//

#include "MyThread.h"
#include <cstring>
MyThread::MyThread(char *name, u32 stake, osPriority priority,
                   os_pthread function, void* arg) {

  attr.name = (char *)pvPortMalloc(strlen(name));
  strcpy(attr.name, name);
  attr.stacksize = stake;
  attr.tpriority = priority;
  attr.pthread = function;
  attr.instances = 0;
  attr.controlblock = NULL;
  attr.buffer = NULL;

  id = osThreadCreate(&attr,arg);
  osThreadSuspend(id);
}
void MyThread::Run() {

  osThreadResume(id);
}
osThreadId MyThread::GetId() { return id; }
