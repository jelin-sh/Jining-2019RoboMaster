//
// Created by Administrator on 2019/2/25.
//

#ifndef STM32_PLUS_UART_H
#define STM32_PLUS_UART_H

#include "stm32plus.h"
#include "Plus_List.h"


class Uart
{
private:
    UART_HandleTypeDef* uartHandler;

public:
    Uart(UART_HandleTypeDef* _uartHandler);

    STM32Plus_Status sendData(u8* data);

    UART_HandleTypeDef* getHandler();
    /*用户可以继承此类,重写此方法*/
    virtual void onRXData(u8 ch);
    virtual void openReceiveIT();
    virtual void openReceiveDMA();

};

class UartList
{
private:
    Plus_List<Uart*>* uartList;
public:
    UartList();
    u16 add(Uart* data);
    Uart* get(u16 index);
    Uart* get(UART_HandleTypeDef* handle);
    u16 index(Uart* data);
    u16 length();
};

extern UartList* uartHandles;

#endif
