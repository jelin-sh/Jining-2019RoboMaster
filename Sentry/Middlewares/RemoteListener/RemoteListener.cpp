//
// Created by Administrator on 2019/4/7.
//

#include <Remote.h>
#include <usart.h>
#include <cmsis_os.h>
#include <MotorControlSystem.h>
#include "RemoteListener.h"

Remote* remote;
RC_CTL RC_Data = {0};
RC_CTL RC_LastData = {0};
bool online;
osThreadId  remoteThreadID;

void RemoteListenerCall(void const* argument);

void RemoteListener::Init()
{
  remote = new Remote(&huart1);
  remote->openReceiveDMA();
  online = false;
  osThreadDef(remoteThread,RemoteListenerCall,osPriorityNormal,0,512);
  remoteThreadID = osThreadCreate(osThread(remoteThread),NULL);
}

void RemoteListener::ReceiveData()
{
  RC_LastData = RC_Data;

  RC_Data.rc.ch0 = static_cast<u16>(((int16_t)remote->rxBuf[0] | ((int16_t)remote->rxBuf[1] << 8)) & 0x07FF);
  RC_Data.rc.ch1 = static_cast<u16>((((int16_t)remote->rxBuf[1] >> 3) | ((int16_t)remote->rxBuf[2] << 5)) & 0x07FF);
  RC_Data.rc.ch2 = static_cast<u16>((((int16_t)remote->rxBuf[2] >> 6) | ((int16_t)remote->rxBuf[3] << 2) |
                                     ((int16_t)remote->rxBuf[4] << 10)) & 0x07FF);
  RC_Data.rc.ch3 = static_cast<u16>((((int16_t)remote->rxBuf[4] >> 1) | ((int16_t)remote->rxBuf[5] << 7)) & 0x07FF);

  RC_Data.rc.s1 = static_cast<u8>(((remote->rxBuf[5] >> 4) & 0x000C) >> 2);
  RC_Data.rc.s2 = static_cast<u8>((remote->rxBuf[5] >> 4) & 0x0003);

  RC_Data.mouse.x = ((int16_t)remote->rxBuf[6]) | ((int16_t)remote->rxBuf[7] << 8);
  RC_Data.mouse.y = ((int16_t)remote->rxBuf[8]) | ((int16_t)remote->rxBuf[9] << 8);
  RC_Data.mouse.z = ((int16_t)remote->rxBuf[10]) | ((int16_t)remote->rxBuf[11] << 8);

  RC_Data.mouse.press_l = static_cast<u8>(remote->rxBuf[12]);
  RC_Data.mouse.press_r = static_cast<u8>(remote->rxBuf[13]);

  RC_Data.key.val = static_cast<u16>((int16_t)remote->rxBuf[14]);

  online = (RC_Data.rc.ch0 >= REMOTE_MIN_VAL) && (RC_Data.rc.ch0 <= REMOTE_MAX_VAL);

}

bool RemoteListener::IsOnline()
{
  if(RC_Data.rc.ch0 < REMOTE_MIN_VAL || RC_Data.rc.ch0 > REMOTE_MAX_VAL)
  {
    return false;
  }
  if(RC_Data.rc.ch1 < REMOTE_MIN_VAL || RC_Data.rc.ch1 > REMOTE_MAX_VAL)
  {
    return false;
  }
  if(RC_Data.rc.ch2 < REMOTE_MIN_VAL || RC_Data.rc.ch2 > REMOTE_MAX_VAL)
  {
    return false;
  }
  if(RC_Data.rc.ch3 < REMOTE_MIN_VAL || RC_Data.rc.ch3 > REMOTE_MAX_VAL)
  {
    return false;
  }

  if(RC_Data.rc.s1 == 0)
  {
    return false;
  }

  return RC_Data.rc.s2 != 0;
}

bool RemoteListener::HaveKey(KEY key)
{
  if(RC_Data.key.val == (u8)key)
  {
    return true;
  }
  return (RC_Data.key.val & (u8)key) != 0;
}

void RemoteListener::printRC()
{
  System::pc::log("RC_Data","%d  %d  %d  %d  %d  %d  %d  %d  %d  %d  %d  %d",
                  RC_Data.rc.ch0,RC_Data.rc.ch1,RC_Data.rc.ch2,RC_Data.rc.ch3,RC_Data.rc.s1,RC_Data.rc.s2,
                  RC_Data.mouse.x, RC_Data.mouse.y, RC_Data.mouse.z, RC_Data.mouse.press_l, RC_Data.mouse.press_r,
                  RC_Data.key.val);
}

void RemoteListener::testKey()
{
  switch (RC_Data.key.val)
  {
    case (u8)KEY::KEY_W :System::pc::log("TestKey","W");break;
    case (u8)KEY::KEY_S:System::pc::log("TestKey","S");break;
    case (u8)KEY::KEY_A:System::pc::log("TestKey","A");break;
    case (u8)KEY::KEY_D:System::pc::log("TestKey","D");break;
    case (u8)KEY::KEY_Q:System::pc::log("TestKey","Q");break;
    case (u8)KEY::KEY_E:System::pc::log("TestKey","E");break;
    case (u8)KEY::KEY_Shift:System::pc::log("TestKey","Shift");break;
    case (u8)KEY::KEY_Ctrl:System::pc::log("TestKey","Ctrl");break;
  }
}

RC_CTL RemoteListener::GetData()
{
  return RC_Data;
}

RC_CTL RemoteListener::GetLastData()
{
  return RC_LastData;
}
bool RemoteListener::LastHaveKey(KEY key) {

  if(RC_LastData.key.val == (u8)key)
  {
    return true;
  }
  return (RC_LastData.key.val & (u8)key) != 0;

}

void RemoteListenerCall(void const* argument)
{
  for(;;)
  {
    RemoteListener::ReceiveData();
    //if(RemoteListener::IsOnline())
    {
      MotorControlSystem::UpdateControlParm();
    }
    osDelay(REMOTE_DELAY_CONST);
  }
}