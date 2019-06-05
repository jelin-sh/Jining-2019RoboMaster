//
// Created by Administrator on 2019/2/27.
//

#ifndef ROBOT_INFENTRY_PID_H
#define ROBOT_INFENTRY_PID_H

#include "stm32plus.h"

class PID
{
protected:
    float target;							//目标值
    float kp;
    float ki;
    float kd;

    float   measure;					    //测量值
    float   err;							//误差
    float   last_err;      		            //上次误差

    float pout;
    float iout;
    float dout;

    float output;						    //本次输出
    float last_output;			            //上次输出

    float MaxOutput;				        //输出限幅
    float IntegralLimit;		            //积分限幅
    float DeadBand;			                //死区（绝对值）


public:

    PID(uint16_t maxOutput,
        uint16_t integralLimit,
        float deadband,
        int16_t  target,
        float kp,
        float ki,
        float kd);

    void reset(float kp,float ki, float kd);
    float calculate(float measure);

    void setTarget(float _target);
    void setMeasure(float _measure);
    void setOutMax(uint16_t maxOutput);

    float getOutMax();
    float getOutput();
    float getTarget();
    float getkp();
    float getki();
    float getkd();

    void clear();
};


#endif //ROBOT_INFENTRY_PID_H
