//
// Created by Administrator on 2019/4/24.
//

#ifndef INFENTERY_BLUETOOTH_H
#define INFENTERY_BLUETOOTH_H


#include <uart/uart.h>

#define BLUETOOTH_RX_SIZE 50

class BlueTooth
{
public:
    static void Init(UART_HandleTypeDef);
    static bool isRxData();
    static void GetData(u8* buf);

};


#endif //INFENTERY_BLUETOOTH_H
