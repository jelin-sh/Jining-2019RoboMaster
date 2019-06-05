//
// Created by Administrator on 2019/4/7.
//

#ifndef INFENTERY_HOLDERCONTROLUNIT_H
#define INFENTERY_HOLDERCONTROLUNIT_H

#include "stm32plus.h"

#define HOLDER_PITCH_ANGLE_MAX 74
#define HOLDER_PITCH_ANGLE_MID 60
#define HOLDER_PITCH_ANGLE_MIN 40

#define HOLDER_YAW_ANGLE_MAX 330
#define HOLDER_YAW_ANGLE_MID 295
#define HOLDER_YAW_ANGLE_MIN 250

#define DEBUG_HOLDER_YAW  0
#define DEBUG_HOLDER_PITCH 0
#define DEBUG_HOLDER_ANGLE 0

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
