//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_CANTXBUF_H
#define INFENTERY_CANTXBUF_H


#include "stm32plus.h"

class CanTxBuf
{
private:
    u8 buf[8];
public:
    void Set(u8 num,u8 data);
    u8* Get();
};


#endif //INFENTERY_CANTXBUF_H
