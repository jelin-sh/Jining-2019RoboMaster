//
// Created by Administrator on 2019/3/8.
//

#ifndef ROBOT_INFENTRY_PWM_H
#define ROBOT_INFENTRY_PWM_H

#include "stm32plus.h"

class PWM
{
private:
    TIM_HandleTypeDef* htim;
    u32 ch;
public:
    PWM(TIM_HandleTypeDef* handle,u32 channel);
    void out(u16 val);
    void start();
    void stop();
};


#endif //ROBOT_INFENTRY_PWM_H
