//
// Created by Administrator on 2019/3/10.
//

#ifndef ROBOT_INFENTRY_IMU_H
#define ROBOT_INFENTRY_IMU_H

extern "C"
{
    #include "bsp_imu.h"
};

#include "stm32plus.h"

class AttitudeListener
{
public:
    static void Init();
    static void updata();
    static bool IsBusy();
    static imu_t* getImu();
};


#endif //ROBOT_INFENTRY_IMU_H
