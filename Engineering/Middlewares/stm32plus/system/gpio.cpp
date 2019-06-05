//
// Created by Administrator on 2019/2/20.
//
#include "System.h"

GPIO_PinState System::gpio::out(GPIO_TypeDef* gpio_typeDef, uint16_t pin, GPIO_PinState val) {
    HAL_GPIO_WritePin(gpio_typeDef,pin,val);
    return get(gpio_typeDef,pin);
}

GPIO_PinState System::gpio::get(GPIO_TypeDef* gpio_typeDef, uint16_t pin) {
    return HAL_GPIO_ReadPin(gpio_typeDef,pin);
}

GPIO_PinState System::gpio::reversal(GPIO_TypeDef *gpio_typeDef, uint16_t pin) {
    HAL_GPIO_TogglePin(gpio_typeDef,pin);
    return get(gpio_typeDef,pin);
}


