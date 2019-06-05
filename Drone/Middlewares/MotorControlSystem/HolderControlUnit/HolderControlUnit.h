//
// Created by Administrator on 2019/4/7.
//

#ifndef INFENTERY_HOLDERCONTROLUNIT_H
#define INFENTERY_HOLDERCONTROLUNIT_H

#include "stm32plus.h"

#define HOLDER_PITCH_ANGLE_MAX 110
#define HOLDER_PITCH_ANGLE_MID 90
#define HOLDER_PITCH_ANGLE_MIN 70

#define HOLDER_YAW_ANGLE_MAX 263
#define HOLDER_YAW_ANGLE_MID 210
#define HOLDER_YAW_ANGLE_MIN 168

class HolderControlUnit
{
public:
    static void Init();

    static void UpdateCanParm();
    static void UpdateControlParmForPC ();
    static void UpdateControlParmForRemote ();

    static void RunYaw(float angle);
    static void RunPitch(float angle);
    static void RunHolderForControlParm ();

    static bool IsOnline();
};


#endif //INFENTERY_HOLDERCONTROLUNIT_H
