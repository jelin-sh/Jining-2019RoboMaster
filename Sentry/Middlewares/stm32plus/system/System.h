//
// Created by Administrator on 2019/2/20.
//

#ifndef STM32_PLUS_SYSTEM_H
#define STM32_PLUS_SYSTEM_H

#include "stm32plus.h"

class System {
public:
    class gpio {
    public:
        static GPIO_PinState out(GPIO_TypeDef* gpio_typeDef, uint16_t pin, GPIO_PinState val);
        static GPIO_PinState get(GPIO_TypeDef* gpio_typeDef,uint16_t pin);
        static GPIO_PinState reversal(GPIO_TypeDef* gpio_typeDef, uint16_t pin);

    };

    class pc
    {
    public:
        static void setHandler(UART_HandleTypeDef* _hUartHandler);
        static int printf(char * fmt, ...);
        static int println(char * fmt, ...);
        static int log(char* log,char * fmt, ...);
    };

};


#endif //STM32_PLUS_SYSTEM_H
