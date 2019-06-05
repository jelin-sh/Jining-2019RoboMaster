//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_MOTORCONTROLSYSTEM_H
#define INFENTERY_MOTORCONTROLSYSTEM_H

class MotorControlSystem
{
public:
    static void Init();
    static void UpdateCanParm();
    static void UpdateControlParm();

    static bool ChassisOnline();
    static bool HolderOnline();
    static bool ShootOnline();

    static void RunChassis();
    static void RunHolder();
    static void RunShoot();
};


#endif //INFENTERY_MOTORCONTROLSYSTEM_H
