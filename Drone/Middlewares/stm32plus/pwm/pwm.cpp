//
// Created by Administrator on 2019/3/8.
//

#include "pwm.h"

PWM::PWM(TIM_HandleTypeDef *handle, u32 channel)
{
    htim = handle;
    ch = channel;

}

void PWM::out(u16 val)
{
    __HAL_TIM_SetCompare(htim,ch,val);
}

void PWM::start()
{
    HAL_TIM_PWM_Start(htim,ch);
}

void PWM::stop()
{
    HAL_TIM_PWM_Stop(htim,ch);
}


