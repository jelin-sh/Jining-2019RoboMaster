//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_CHASSISCONTROLUNIT_H
#define INFENTERY_CHASSISCONTROLUNIT_H

#include "stm32plus.h"

#define CHASSIS_SPEED_MAX  5000
#define CHASSIS_SPEED_BASE 	(660.0/CHASSIS_SPEED_MAX)       //底盘增量速度
#define CHASSIS_POWER_WARNING 80                            //限定功率(W)
#define CHASSIS_BASE_CURRENT 600                            //最大功率平均输出值

#define MOTOR_CHASSIS1 0
#define MOTOR_CHASSIS2 1
#define MOTOR_CHASSIS3 2
#define MOTOR_CHASSIS4 3

typedef struct
{
    int forward_back_ch;
    int left_right_ch;
    int rotate_ch;
} SpeedStruct;

class ChassisControlUnit
{
public:


    static void Init();

    static void RunMotor(u16 port,short speed);
    static void RunMotorForControlParm ();

    static void UpdateCanParm();
    static void UpdateControlParmForRemote ();
    static void UpdateControlParmForPC ();

    static bool IsOnline();
    static bool IsRevolve();


};


#endif //INFENTERY_CHASSISCONTROLUNIT_H
