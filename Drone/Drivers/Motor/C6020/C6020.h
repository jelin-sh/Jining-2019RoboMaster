//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_C6020_H
#define INFENTERY_C6020_H


#include <stm32plus.h>
#include <PID.h>
#include <CanListener/CanListener.h>
#include <Kalman.h>

class C6020
{
protected:
    //参数:转速 转矩 转角度
    short mSpeed;
    short mTorque;
    float mAngle;
    //CAN Port
    u16 mPort;
    CanListener* canListener;
    //ControlArithmetic
    PID* mPID;
public:
    C6020(u16 port,PID* pid);

    short GetSpeed();
    short GetTorque();
    float GetAngel();

    CanListener* GetListener();

    virtual void UpdateParm();

    virtual void printParm();

    short CalcOutput(short targetSpeed);
    void SetMaxOut(float maxOut);
};


#endif //INFENTERY_C6020_H
