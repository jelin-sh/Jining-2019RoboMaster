//
// Created by Administrator on 2019/2/28.
//

#include "Remote.h"

void Remote::openReceiveDMA()
{
    HAL_UART_DMAStop(getHandler());
    HAL_UART_Receive_DMA(getHandler(),(u8*)rxBuf,18);
}

Remote::Remote(UART_HandleTypeDef *_uartHandler) : Uart(_uartHandler)
{

}
