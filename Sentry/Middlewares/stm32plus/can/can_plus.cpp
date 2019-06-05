//
// Created by Administrator on 2019/3/5.
//

#include <cstring>
#include "can_plus.h"
CAN_RxMessage listMessage;

void UserCAN_Init(CAN_HandleTypeDef* canHandle)
{
  CAN_FilterTypeDef filter;
  filter.FilterIdHigh=0;
  filter.FilterIdLow=0;
  filter.FilterMaskIdHigh=0;
  filter.FilterMaskIdLow=0;
  filter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
  filter.SlaveStartFilterBank=14;
  filter.FilterMode=CAN_FILTERMODE_IDMASK;
  filter.FilterScale=CAN_FILTERSCALE_32BIT;
  filter.FilterActivation=ENABLE;
  filter.FilterBank=0;
  if(HAL_CAN_ConfigFilter(&hcan1,&filter) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_CAN_Start(canHandle) != HAL_OK)
  {
    Error_Handler();
  }
}
Can::Can(CAN_HandleTypeDef *hcan)
{
  canHandle = hcan;
  UserCAN_Init(canHandle);
}
CAN_HandleTypeDef *Can::getHandle()
{
    return canHandle;
}

u16 Can::ReceiveData(u8* buf)
{
  HAL_CAN_GetRxMessage(canHandle, CAN_RX_FIFO0, &RxHandle.rxMessage, RxHandle.data);
  if(RxHandle.rxMessage.DLC == 8)
  {
    //memcpy(buf,RxHandle.data,8);
    for(int i = 0;i<8;i++)
    {
      buf[i] = RxHandle.data[i];
    }

  }
  return RxHandle.rxMessage.StdId;
}

u8 Can::SendData(u16 id, u8 *msg)
{
  CAN_TxHeaderTypeDef txHandel;
  txHandel.StdId = id;
  txHandel.DLC = 8;
  txHandel.IDE = CAN_ID_STD;
  txHandel.RTR = CAN_RTR_DATA;
  txHandel.TransmitGlobalTime = ENABLE;
  while((canHandle->Instance->TSR & CAN_TSR_TME0) == RESET);
  if(HAL_CAN_AddTxMessage(canHandle,&txHandel,msg,(uint32_t*)CAN_TX_MAILBOX0) != HAL_OK)
  {
    System::pc::println("0x%02x send data err:0x%02x",txHandel.StdId,canHandle->ErrorCode);
  }
  return 0;
}


