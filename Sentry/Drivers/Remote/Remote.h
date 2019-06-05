//
// Created by Administrator on 2019/2/28.
//

#ifndef ROBOT_INFENTRY_REMOTE_H
#define ROBOT_INFENTRY_REMOTE_H

#include "stm32plus.h"
#include "uart.h"

class Remote:public Uart
{
public:
    char rxBuf[18] = {0};
public:
    void openReceiveDMA() override;
public:
    Remote(UART_HandleTypeDef *_uartHandler);

};


#endif //ROBOT_INFENTRY_REMOTE_H
