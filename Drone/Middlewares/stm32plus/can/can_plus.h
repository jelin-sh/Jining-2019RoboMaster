//
// Created by Administrator on 2019/3/5.
//

#ifndef ROBOT_INFENTRY_CAN_H
#define ROBOT_INFENTRY_CAN_H

#include "stm32plus.h"
#include <can.h>
class Can
{
private:
    CAN_HandleTypeDef* canHandle;
public:
    explicit Can(CAN_HandleTypeDef* hcan);
    u16 ReceiveData(u8* buf);
    u8 SendData(u16 id, u8* msg);
    u32 GetFreeTxMail();
    CAN_HandleTypeDef* getHandle();
};


#endif //ROBOT_INFENTRY_CAN_H
