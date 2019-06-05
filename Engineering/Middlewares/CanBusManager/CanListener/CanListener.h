//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_CANLISTENER_H
#define INFENTERY_CANLISTENER_H

#include "stm32plus.h"
#include <can/can_plus.h>
#include <CanPort/CanPort.h>

class CanListener
{
private:
    CanPort* canPort;
public:
    CanListener(u16 portId);
    u8* GetData();
    void WriteData(short data);
    bool isOnline();
};


#endif //INFENTERY_CANLISTENER_H
