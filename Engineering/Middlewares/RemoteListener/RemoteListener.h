//
// Created by Administrator on 2019/4/7.
//

#ifndef INFENTERY_REMOTELISTENER_H
#define INFENTERY_REMOTELISTENER_H

#include "stm32plus.h"

#define REMOTE_MAX_VAL 1684
#define REMOTE_MID_VAL 1024
#define REMOTE_MIN_VAL 364
#define REMOTE_DELAY_CONST 15
#define REMOTE_THREAD_DELAY(TIM) (TIM/REMOTE_DELAY_CONST)


typedef struct
{
    u16 ch0:11;
    u16 ch1:11;
    u16 ch2:11;
    u16 ch3:11;
    u8	s1:2;
    u8	s2:2;
}rcData;

typedef struct
{
    short x;
    short y;
    short z;
    u8 press_l;
    u8 press_r;
}mouseData;

typedef struct
{
    u16 val;
}keyData;

typedef struct __RC_Ctl
{
    rcData rc;
    mouseData mouse;
    keyData key;
}RC_CTL;

enum class KEY
{
    KEY_W = 0x01,
    KEY_S = 0x02,
    KEY_A = 0x04,
    KEY_D = 0x08,
    KEY_Q = 0x40,
    KEY_E = 0x80,
    KEY_Shift = 0x10,
    KEY_Ctrl = 0x20,
    KEY_NULL = 0x00
};

class RemoteListener
{
public:
    static void Init();
    static void ReceiveData();

    static bool IsOnline();
    static bool HaveKey(KEY key);
    static bool LastHaveKey(KEY key);
    static void printRC();
    static void testKey();

    static RC_CTL GetData();
    static RC_CTL GetLastData();
};


#endif //INFENTERY_REMOTELISTENER_H
