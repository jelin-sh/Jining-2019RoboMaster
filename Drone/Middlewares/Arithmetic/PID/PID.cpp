//
// Created by Administrator on 2019/2/27.
//

#include "stm32f4xx_hal.h"
#include "PID.h"

#define ABS(x)		((x>0)? x: -x)

void PID::reset(float kp, float ki, float kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

float PID::calculate(float measure)
{

    this->measure = measure;

    this->last_err  = this->err;
    this->last_output = this->output;

    this->err = this->target - this->measure;

    //是否进入死区
    if((ABS(this->err) > this->DeadBand))
    {
        this->pout = this->kp * this->err;
        this->iout += (this->ki * this->err);


        this->dout = this->kd * (this->err - this->last_err);

        //积分是否超出限制
        if (this->iout > this->IntegralLimit)
            this->iout = this->IntegralLimit;
        if (this->iout < -this->IntegralLimit)
            this->iout = -this->IntegralLimit;

        //this输出和
        this->output = this->pout + this->iout + this->dout;


        //this->output = this->output*0.7f + this->last_output*0.3f;  //滤波？
        if (this->output > this->MaxOutput)
        {
            this->output = this->MaxOutput;
        }
        if (this->output < -(this->MaxOutput))
        {
            this->output = -(this->MaxOutput);
        }
    }
    return this->output;

}

PID::PID(uint16_t maxOutput, uint16_t integralLimit, float deadband,
         int16_t target, float kp, float ki, float kd)
{
    this->DeadBand = deadband;
    this->IntegralLimit = integralLimit;
    this->MaxOutput = maxOutput;
    this->target = target;

    this->kp = kp;
    this->ki = ki;
    this->kd = kd;

    this->output = 0;
    this->iout = 0;
    this->pout = 0;
    this->dout = 0;
}

void PID::setTarget(float _target)
{
    this->target = _target;
}

void PID::setMeasure(float _measure)
{
    this->measure = _measure;
}

float PID::getOutput()
{
    return this->output;
}

float PID::getTarget()
{
    //System::pc::printf("%d  ",(short)target);
    return this->target;
}

float PID::getkp()
{
    return kp;
}

float PID::getki()
{
    return ki;
}

float PID::getkd()
{
    return kd;
}

void PID::setOutMax(uint16_t maxOutput)
{
    MaxOutput = maxOutput;
}

float PID::getOutMax()
{
    return MaxOutput;
}

void PID::clear()
{
    target = 0;
    measure = 0;
    output = 0;
    pout = 0;
    iout = 0;
    dout = 0;
}


