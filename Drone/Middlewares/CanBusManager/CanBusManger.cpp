//
// Created by Administrator on 2019/4/6.
//

#include <can/can_plus.h>
#include <can.h>
#include <CanPort/CanPort.h>
#include <List/Plus_List.h>
#include <cmsis_os.h>
#include <MotorControlSystem.h>
#include "CanBusManger.h"
#include "CanPortList.h"

CanPortList* canPortList;
Can* canBus1;
Can* canBus2;
CanTxBuf can1TxBuf_200; //ID: 201~204
CanTxBuf can1TxBuf_1ff; //ID: 205~209
CanTxBuf can1TxBuf_2ff; //ID: 20A~20B

CanTxBuf can2TxBuf_200; //ID: 201~204
CanTxBuf can2TxBuf_1ff; //ID: 205~209
CanTxBuf can2TxBuf_2ff; //ID: 20A~20B

u8 can1RxBuf[8];
u8 can2RxBuf[8];

osThreadId canBusThreadID;

/**CAN1总线要监听的ID Port列表**/
const u8 can1IdSize = 2;
const u8 can2IdSize = 1;

u16 Can1RxIDList[can1IdSize] = {
        CAN_ID_SHOOT,
        CAN_ID_HODERYAW,
};

u16 Can2RxIDList[can2IdSize] = {
    CAN_ID_HODERPITCH
};

void CanBusMangerCall(void const * argument);

void printData(u8* data)
{
  for(int i = 0;i<8;i++)
  {
    System::pc::printf("0x%02x ",data[i]);
  }
  System::pc::printf("\r\n");
}

void CanBusManger::Init()
{
  canBus1 = new Can(&hcan1);
  canBus2 = new Can(&hcan2);
  canPortList = new CanPortList();

  for(u16 port : Can1RxIDList)
  {
    canPortList->add(new CanPort(port));
  }
  for(u16 port : Can2RxIDList)
  {
    canPortList->add(new CanPort(port));
  }

  osThreadDef(canBusTask,CanBusMangerCall,osPriorityAboveNormal,0,1500);
  canBusThreadID = osThreadCreate(osThread(canBusTask),NULL);
}
CanBusStatus CanBusManger::Update()
{
  u16 id1 = canBus1->ReceiveData(can1RxBuf);
  u16 id2 = canBus2->ReceiveData(can2RxBuf);
//  System::pc::println("ID:%d",id);

  canPortList->getForId(id1)->ReceiveData(can1RxBuf);
  canPortList->getForId(id2)->ReceiveData(can2RxBuf);

  return CAN_BUS_UPDATA_OK;
}

void CanBusManger::SendData_200()
{
  canBus1->SendData(0x200,can1TxBuf_200.Get());
  canBus2->SendData(0x200,can2TxBuf_200.Get());
}

void CanBusManger::SendData_1ff()
{
  canBus1->SendData(0x1ff,can1TxBuf_1ff.Get());
  canBus2->SendData(0x1ff,can2TxBuf_1ff.Get());
}

void CanBusManger::SendData_2ff()
{
  canBus1->SendData(0x2ff,can1TxBuf_2ff.Get());
  canBus2->SendData(0x2ff,can2TxBuf_2ff.Get());
}

CanPortList* CanBusManger::GetPortList()
{
  return canPortList;
}

void CanBusManger::TxBufUpdate(u16 id)
{
  switch (id)
  {
    case 0x1ff:
      for(int i = 0;i<8;i+=2)
      {
        CanPort* port = canPortList->getForId(CAN_ID_1FF_MSR+(i/2));
        if(port != nullptr)
        {
          if(IsCan1Port(port->GetID()))
          {
            can1TxBuf_1ff.Set(i,port->GetTxData()[0]);
            can1TxBuf_1ff.Set(i+1,port->GetTxData()[1]);
          }
          else if(IsCan2Port(port->GetID()))
          {
            can2TxBuf_1ff.Set(i,port->GetTxData()[0]);
            can2TxBuf_1ff.Set(i+1,port->GetTxData()[1]);
          }
        }
      }
      break;
    case 0x200:
      for(int i = 0;i<8;i+=2)
      {
        CanPort* port = canPortList->getForId(CAN_ID_200_MSR+(i/2));
        if(port != nullptr)
        {
          if(IsCan1Port(port->GetID()))
          {
            can1TxBuf_200.Set(i,port->GetTxData()[0]);
            can1TxBuf_200.Set(i+1,port->GetTxData()[1]);
          }
          else if(IsCan2Port(port->GetID()))
          {
            can2TxBuf_200.Set(i,port->GetTxData()[0]);
            can2TxBuf_200.Set(i+1,port->GetTxData()[1]);
          }
        }
      }
      break;
    case 0x2ff:
      for(int i = 0;i<8;i+=2)
      {
        CanPort* port = canPortList->getForId(CAN_ID_2FF_MSR+(i/2));
        if(port != nullptr)
        {
          if(IsCan1Port(port->GetID()))
          {
            can1TxBuf_2ff.Set(i,port->GetTxData()[0]);
            can1TxBuf_2ff.Set(i+1,port->GetTxData()[1]);
          }
          else if(IsCan2Port(port->GetID()))
          {
            can2TxBuf_2ff.Set(i,port->GetTxData()[0]);
            can2TxBuf_2ff.Set(i+1,port->GetTxData()[1]);
          }
        }
      }
      break;

  }
}

void CanBusManger::printDriver()
{
  for(int i = 0;i<8;i++)
  {
    System::pc::printf("Driver 0x%02x: ",canPortList->get(i)->GetID());
    if(canPortList->get(i)->isOnline())
    {
      System::pc::println("online");
    }
    else
    {
      System::pc::println("offline");
    }
  }
}
bool CanBusManger::IsCan1Port(u16 can_port) {

  for(u16 port : Can1RxIDList)
  {
    if(can_port == port)
    {
      return true;
    }
  }
  return false;
}
bool CanBusManger::IsCan2Port(u16 can_port) {
  for(u16 port : Can2RxIDList)
  {
    if(can_port == port)
    {
      return true;
    }
  }
  return false;
}

void CanBusMangerCall(void const * argument)
{
  bool chackSign = false;
  for(u16 count = 0;;count++)
  {
    /*****更新下行总线数据******/
    CanBusManger::Update();

    MotorControlSystem::UpdateCanParm();

    /*******更新数据总线********/
    CanBusManger::TxBufUpdate(0x200);
    CanBusManger::TxBufUpdate(0x1ff);
    CanBusManger::TxBufUpdate(0x2ff);

    /********提交数据**********/
    CanBusManger::SendData_1ff();
    CanBusManger::SendData_200();
    CanBusManger::SendData_2ff();

    /********查看在线设备**********/
    count++;
    if(count>10000 && !chackSign)
    {
      CanBusManger::printDriver();
      chackSign = true;
    }
    osDelay(1);
  }
}