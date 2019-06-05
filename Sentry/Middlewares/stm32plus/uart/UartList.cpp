//
// Created by Administrator on 2019/2/26.
//

#include "uart.h"
#include "Plus_List.cpp"



UartList::UartList()
{
    uartList = new Plus_List<Uart*>();
}

u16 UartList::add(Uart *data)
{
    return uartList->add(data);
}

Uart *UartList::get(u16 index)
{
    return uartList->get(index);;
}

Uart *UartList::get(UART_HandleTypeDef *handle)
{
    for(u16 i = 0;i<length();i++)
    {
        if(get(i)->getHandler() == handle)
        {
            return get(i);
        }
    }
    return nullptr;
}

u16 UartList::index(Uart *data)
{
    return uartList->index(data);;
}

u16 UartList::length()
{
    return uartList->length();;
}

