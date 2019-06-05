//
// Created by Administrator on 2019/5/4.
//

#include "usart.h"
#include <CourtSystem.h>
#include <MiniPC.h>

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == UART7)
  {
    CourtSystem::SerialPortRxCallBack();
    return;
  }
  if(huart->Instance == UART8)
  {
    MiniPC::MiniPCRxCall();
    return;
  }
}

