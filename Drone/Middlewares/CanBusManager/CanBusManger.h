//
// Created by Administrator on 2019/4/6.
//

#ifndef INFENTERY_CANBUSMANGER_H
#define INFENTERY_CANBUSMANGER_H

#include "stm32plus.h"
#include "CanTxBuf.h"
#include "CanPortList.h"
#include "can_plus.h"

#define CAN_ID_Motor_1    0x201
#define CAN_ID_Motor_2    0x202
#define CAN_ID_Motor_3    0x203
#define CAN_ID_Motor_4    0x204
#define CAN_ID_Motor_5    0x205
#define CAN_ID_Motor_6    0x206
#define CAN_ID_Motor_7    0x207
#define CAN_ID_Motor_8    0x208
#define CAN_ID_Motor_9    0x209
#define CAN_ID_Motor_10   0x20A
#define CAN_ID_Motor_11   0x20B

#define CAN_ID_POWER      0x250
#define CAN_ID_CHASSIS1   CAN_ID_Motor_1
#define CAN_ID_CHASSIS2   CAN_ID_Motor_2
#define CAN_ID_CHASSIS3   CAN_ID_Motor_3
#define CAN_ID_CHASSIS4   CAN_ID_Motor_4
#define CAN_ID_SHOOT      CAN_ID_Motor_3
#define CAN_ID_HODERPITCH CAN_ID_Motor_9
#define CAN_ID_HODERYAW   CAN_ID_Motor_7

#define CAN_ID_200_MSR    CAN_ID_Motor_1
#define CAN_ID_200_LSR    CAN_ID_Motor_4
#define CAN_ID_1FF_MSR    CAN_ID_Motor_5
#define CAN_ID_1FF_LSR    CAN_ID_Motor_8
#define CAN_ID_2FF_MSR    CAN_ID_Motor_9
#define CAN_ID_2FF_LSR    CAN_ID_Motor_11

typedef enum
{
    CAN_BUS_UPDATA_ERR = 0,
    CAN_BUS_UPDATA_OK
}CanBusStatus;

extern Can* canBus1;
extern CanTxBuf can1TxBuf_200;
extern CanTxBuf can1TxBuf_1ff;
extern CanTxBuf can1TxBuf_2ff;

extern Can* canBus2;
extern CanTxBuf can2TxBuf_200;
extern CanTxBuf can2TxBuf_1ff;
extern CanTxBuf can2TxBuf_2ff;

class CanBusManger
{
private:
    static bool IsCan1Port(u16 can_port);
    static bool IsCan2Port(u16 can_port);
public:
    static void Init();
    static CanBusStatus Update();
    static void SendData_200();
    static void SendData_1ff();
    static void SendData_2ff();
    static void TxBufUpdate(u16 id);
    static CanPortList* GetPortList();

    static void printDriver();

};
#endif //INFENTERY_CANBUSMANGER_H
