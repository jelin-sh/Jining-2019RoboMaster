//
// Created by Administrator on 2019/2/25.
//

#include "uart.h"


UartList* uartHandles = new UartList();

Uart::Uart(UART_HandleTypeDef *_uartHandler)
{
    this->uartHandler = _uartHandler;
    uartHandles->add(this);
}

STM32Plus_Status Uart::sendData(u8 *data)
{
    HAL_UART_Transmit(uartHandler,data,sizeof(data),20);
    return STM32PLUS_ERR;
}

UART_HandleTypeDef *Uart::getHandler()
{
    return uartHandler;
}

void Uart::onRXData(u8 ch)
{

}

void Uart::openReceiveIT()
{

}

void Uart::openReceiveDMA()
{

}

