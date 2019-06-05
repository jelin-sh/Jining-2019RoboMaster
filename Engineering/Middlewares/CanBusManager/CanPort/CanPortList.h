//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_CANPORTLIST_H
#define INFENTERY_CANPORTLIST_H

#include "stm32plus.h"
#include <List/Plus_List.h>
#include <CanPort/CanPort.h>
#include "Plus_List.cpp"
class CanPortList: public Plus_List<CanPort*>
{
public:
    CanPort* getForId(u16 id);
};


#endif //INFENTERY_CANPORTLIST_H
