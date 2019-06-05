//
// Created by Administrator on 2019/3/5.
//

#include "can_plus.h"
#include <cstring>
CAN_RxMessage listMessage;

void UserCAN_Init(CAN_HandleTypeDef *canHandle) {
  CAN_FilterTypeDef filter;
  filter.FilterIdHigh = 0;
  filter.FilterIdLow = 0;
  filter.FilterMaskIdHigh = 0;
  filter.FilterMaskIdLow = 0;
  if (canHandle->Instance == CAN1) {
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank = 0;

  } else {
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    filter.FilterBank = 16;

  }
  filter.SlaveStartFilterBank = 13;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterActivation = ENABLE;

  if (HAL_CAN_ConfigFilter(canHandle, &filter) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_CAN_Start(canHandle) != HAL_OK) {
    Error_Handler();
  }
}
Can::Can(CAN_HandleTypeDef *hcan) {
  canHandle = hcan;
  UserCAN_Init(canHandle);
}
CAN_HandleTypeDef *Can::getHandle() { return canHandle; }

u16 Can::ReceiveData(u8 *buf) {
  CAN_RxMessage *RxHandle =
      canHandle->Instance == CAN1 ? &can1RxHandle : &can2RxHandle;
  if(canHandle->Instance == CAN1){
    HAL_CAN_GetRxMessage(canHandle, CAN_RX_FIFO0, &RxHandle->rxMessage,
                         RxHandle->data);
  } else{
    auto res = HAL_CAN_GetRxMessage(canHandle, CAN_RX_FIFO1, &RxHandle->rxMessage,
                         RxHandle->data);
  }
  if (RxHandle->rxMessage.DLC == 8) {
    // memcpy(buf,RxHandle.data,8);
    for (int i = 0; i < 8; i++) {
      buf[i] = RxHandle->data[i];
    }
  }
  return RxHandle->rxMessage.StdId;
}

u8 Can::SendData(u16 id, u8 *msg) {
  CAN_TxHeaderTypeDef txHandel;
  txHandel.StdId = id;
  txHandel.DLC = 8;
  txHandel.IDE = CAN_ID_STD;
  txHandel.RTR = CAN_RTR_DATA;
  txHandel.TransmitGlobalTime = ENABLE;
  if (HAL_CAN_GetTxMailboxesFreeLevel(canHandle) > 0) {
    if (HAL_CAN_AddTxMessage(canHandle, &txHandel, msg,
                             (uint32_t *)GetFreeTxMail()) != HAL_OK) {
      System::pc::println("0x%02x send data err:0x%02x", txHandel.StdId,
                          canHandle->ErrorCode);
    }
  } else{
    return 1;
  }
  return 0;
}
u32 Can::GetFreeTxMail() {
  if(HAL_CAN_GetTxMailboxesFreeLevel(canHandle) > 0)
  {
    if ((canHandle->Instance->TSR & CAN_TSR_TME0) != 0U)
    {
      return CAN_TX_MAILBOX0;
    }
    if ((canHandle->Instance->TSR & CAN_TSR_TME1) != 0U)
    {
      return CAN_TX_MAILBOX1;
    }
    if ((canHandle->Instance->TSR & CAN_TSR_TME2) != 0U)
    {
      return CAN_TX_MAILBOX2;
    }
  }
  return 0;

}
