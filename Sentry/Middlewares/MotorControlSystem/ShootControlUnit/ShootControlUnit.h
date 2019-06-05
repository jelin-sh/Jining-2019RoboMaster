//
// Created by Administrator on 2019/4/30.
//

#ifndef INFENTERY_SHOOTCONTROLUNIT_H
#define INFENTERY_SHOOTCONTROLUNIT_H

/**
 * IO:
 * ShootMotor1:PI5----------------- W
 * ShootMotor2:PI6----------------- X
 *
 * **/

class ShootControlUnit
{
private:
    static void On();
    static void Off();
public:
    enum class ShootMotorSpeed
    {
        LOW = 0,
        MID,
        HEIGH
    };

    static void Init();
    static void SetShootMotorSpeed(ShootMotorSpeed speed);

    static void OpenShootMotor();
    static void RunShootForControlParm ();

    static void UpdateCanParm();
    static void UpdateControlParmForRemote();
    static void UpdateControlParmForPC();
    static bool IsOnline();



};


#endif //INFENTERY_SHOOTCONTROLUNIT_H
