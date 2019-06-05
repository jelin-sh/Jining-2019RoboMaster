//
// Created by jelin on 19-5-16.
//

#include "MiniPC.h"
#include <cstdio>
#include <cstring>
#include <stm32f4xx_hal_uart.h>
#include <usart.h>

u8 miniPCRxBuf;
bool miniReceiveOK = false;
u32 miniPCOnlineTick = 0;
u8 miniPCRxData[50] = {0};
u16 pointData[2] = {0};

void MiniPC::MiniPCRxCall() {
  static u8 i = 0;
  miniPCOnlineTick = HAL_GetTick();
  HAL_UART_Receive_IT(&huart8, &miniPCRxBuf, 1);
  if (miniReceiveOK) {
    return;
  }
  if (miniPCRxBuf == '\n') {
    i = 0;
    miniReceiveOK = true;
    sscanf(reinterpret_cast<const char *>(miniPCRxData), "c:%hd,%hd",
           &pointData[0], &pointData[1]);
    //System::pc::println("%s",miniPCRxData);
    memset(miniPCRxData,0,50);
    miniReceiveOK = false;
    return;
  }
  miniPCRxData[i] = miniPCRxBuf;
  i++;
}
void MiniPC::Init() { HAL_UART_Receive_IT(&huart8, &miniPCRxBuf, 1); }
void MiniPC::Start() {
  HAL_UART_Transmit(&huart8, (uint8_t *)"c,r,5,1,\n", 10, 50);
}
u16 MiniPC::GetX() { return pointData[0]; }
u16 MiniPC::GetY() { return pointData[1]; }
bool MiniPC::isOnline() {
  u32 Tick = HAL_GetTick();
  u32 dt = Tick - miniPCOnlineTick;
  return (dt<1000);
}
void MiniPC::ClearX() {pointData[0] = 0;}
void MiniPC::ClearY() {pointData[1] = 0;}
