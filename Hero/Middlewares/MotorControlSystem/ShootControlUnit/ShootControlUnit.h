//
// Created by Administrator on 2019/4/30.
//

#ifndef INFENTERY_SHOOTCONTROLUNIT_H
#define INFENTERY_SHOOTCONTROLUNIT_H

/**
 * IO:
 * ShootMotor_s1:PI5----------------- W
 * ShootMotor_s2:PI6----------------- X
 * ShootMotor_b1:PI2------------------Y
 * ShootMotor_b1:PI7------------------Z
 * **/

class ShootControlUnit
{
private:
    static void OnSmall();
    static void OffSmall();
    static void OnBig();
    static void OffBig();

public:
    enum class ShootMotorSpeed
    {
        LOW = 0,
        MID,
        HEIGH
    };

    static void Init();

    static void OpenShootMotor();
    static void RunShootForControlParm ();

    static void UpdateCanParm();
    static void UpdateControlParmForRemote();
    static void UpdateControlParmForPC();
    static bool IsOnline();



};


#endif //INFENTERY_SHOOTCONTROLUNIT_H
