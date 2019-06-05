//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_CANPORT_H
#define INFENTERY_CANPORT_H

#include <stm32plus.h>

class CanPort
{
private:
    u16 id;
    u8 txData[2];
    u8 rxData[8];
    bool online = false;
public:
    CanPort(u16 portId);

    u8* GetRxData();
    u8* GetTxData();
    u16 GetID();
    bool isOnline();
    void ReceiveData(u8* val);
    void WriteData(short val);

    void ClearTxData();
    void ClearRxData();

    void printRxData();
    void printTxData();
};


#endif //INFENTERY_CANPORT_H
