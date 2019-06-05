//
// Created by Administrator on 2019/2/20.
//

#include "Port.h"

Port::Port(GPIO_TypeDef *type, uint16_t pin) {

    this->gpio_type = type;
    this->pin = pin;

}

Port::PortStatus Port::set() {
    System::gpio::out(gpio_type,pin,GPIO_PIN_SET);
    return get();
}

Port::PortStatus Port::reset() {
    System::gpio::out(gpio_type,pin,GPIO_PIN_RESET);
    return get();
}

Port::PortStatus Port::reversal() {
    if(System::gpio::reversal(gpio_type,pin) == GPIO_PIN_RESET)
    {
        return Port_Status_Off;
    }
    return get();
}

Port::PortStatus Port::get()
{
    if(System::gpio::get(gpio_type,pin) == GPIO_PIN_SET)
    {
        return Port_Status_On;
    }
    return Port_Status_Off;
}




