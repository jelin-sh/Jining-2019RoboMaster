//
// Created by Administrator on 2019/4/7.
//

#ifndef INFENTERY_HOLDERCONTROLUNIT_H
#define INFENTERY_HOLDERCONTROLUNIT_H

#include "stm32plus.h"

#define HOLDER_PITCH_ANGLE_MAX 105
#define HOLDER_PITCH_ANGLE_MID 95
#define HOLDER_PITCH_ANGLE_MIN 70

#define HOLDER_YAW_ANGLE_MAX 345
#define HOLDER_YAW_ANGLE_MID 270
#define HOLDER_YAW_ANGLE_MIN 190

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
