//
// Created by Administrator on 2019/2/25.
//
#include <cstdio>
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"
#include "System.h"

UART_HandleTypeDef* hUartHandler;

void System::pc::setHandler(UART_HandleTypeDef *_hUartHandler)
{
    hUartHandler = _hUartHandler;
}

int System::pc::printf(char * fmt, ...)
{
    if(hUartHandler == NULL)
    {
        return 0 ;
    }
    char str[200];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    va_end(ap);
    if(HAL_UART_Transmit(hUartHandler,(uint8_t *)str, (uint16_t)strlen(str),10) == HAL_OK)
    {
        return 1;
    }
    return 0;
}

int System::pc::println(char *fmt, ...)
{
    if(hUartHandler == NULL)
    {
        return 0;
    }
    char str[200];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    va_end(ap);
    strcat(str,"\r\n");
    if(HAL_UART_Transmit(hUartHandler,(uint8_t *)str, (uint16_t)strlen(str),10) == HAL_OK)
    {
        return 1;
    }
    return 0;
}

int System::pc::log(char *log, char *fmt, ...)
{
    if(hUartHandler == NULL)
    {
        return 0;
    }
    char str[200];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    va_end(ap);
    strcat(str,"\r\n");
    HAL_UART_Transmit(hUartHandler,(uint8_t*)"[", 1,20);
    HAL_UART_Transmit(hUartHandler,(uint8_t*)log, (uint16_t)strlen(log),20);
    HAL_UART_Transmit(hUartHandler,(uint8_t*)"]:", 2,20);
    if(HAL_UART_Transmit(hUartHandler,(uint8_t *)str, (uint16_t)strlen(str),20) == HAL_OK)
    {
        return 1;
    }
    return 0;
}

