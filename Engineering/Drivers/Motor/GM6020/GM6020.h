//
// Created by Administrator on 2019/4/7.
//

#ifndef INFENTERY_GM6020_H
#define INFENTERY_GM6020_H


#include <C6020/C6020.h>

class GM6020: public C6020
{
private:
    u8 mTemperature;
    u16 mAngleData;
    PID* mPIDAngel;
public:
    void UpdateParm() override;
    void printParm() override;

    float AnglePIDOut(float angle);
    short SpeedPIDOut(float target,float current);

    u16 GetAngleData();

    void SetAngle(float angle);
    GM6020(u16 port, PID *_pidSpeed, PID *_pidAngle);
};


#endif //INFENTERY_GM6020_H
