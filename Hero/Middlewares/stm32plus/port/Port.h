//
// Created by Administrator on 2019/2/20.
//

#ifndef STM32_PLUS_PORT_H
#define STM32_PLUS_PORT_H

#include "stm32plus.h"

class Port {
private:
    GPIO_TypeDef* gpio_type;
    uint16_t pin;
public:
    typedef enum
    {
        Port_Status_On,
        Port_Status_Off
    }PortStatus;

    PortStatus set();
    PortStatus get();
    PortStatus reset();
    PortStatus reversal();


    explicit Port(GPIO_TypeDef* type, uint16_t pin);
};


#endif //STM32_PLUS_PORT_H
