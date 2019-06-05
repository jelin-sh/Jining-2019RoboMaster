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
CanTxBuf txBuf_200; //ID: 201~204
CanTxBuf txBuf_1ff; //ID: 205~209
CanTxBuf txBuf_2ff; //ID: 20A~20B

u8 rxBuf[8];
osThreadId canBusThreadID;

/**CAN1总线要监听的ID Port列表**/
u16 CanRxIDList[] = {
        CAN_ID_CHASSIS1,
        CAN_ID_CHASSIS2,
        CAN_ID_SHOOT,
        CAN_ID_HODERPITCH,
        CAN_ID_HODERYAW
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
  canPortList = new CanPortList();
  for(unsigned short i : CanRxIDList)
  {
    canPortList->add(new CanPort(i));
  }
  osThreadDef(canBusTask,CanBusMangerCall,osPriorityNormal,0,1024);
  canBusThreadID = osThreadCreate(osThread(canBusTask),NULL);
}
CanBusStatus CanBusManger::Updata()
{
  u16 id = canBus1->ReceiveData(rxBuf);


  canPortList->getForId(id)->ReceiveData(rxBuf);

  return CAN_BUS_UPDATA_OK;
}

void CanBusManger::SendData_200()
{
  canBus1->SendData(0x200,txBuf_200.Get());
}

void CanBusManger::SendData_1ff()
{
  canBus1->SendData(0x1ff,txBuf_1ff.Get());
}

void CanBusManger::SendData_2ff()
{
  canBus1->SendData(0x2ff,txBuf_2ff.Get());
}

CanPortList* CanBusManger::GetPortList()
{
  return canPortList;
}

void CanBusManger::TxBufUpdata(u16 id)
{
  switch (id)
  {
    case 0x1ff:
      for(int i = 0;i<8;i+=2)
      {
        CanPort* port = canPortList->getForId(CAN_ID_1FF_MSR+(i/2));
        if(port != nullptr)
        {
          txBuf_1ff.Set(i,port->GetTxData()[0]);
          txBuf_1ff.Set(i+1,port->GetTxData()[1]);
        }
      }
      break;
    case 0x200:
      for(int i = 0;i<8;i+=2)
      {
        CanPort* port = canPortList->getForId(CAN_ID_200_MSR+(i/2));
        if(port != nullptr)
        {
          txBuf_200.Set(i,port->GetTxData()[0]);
          txBuf_200.Set(i+1,port->GetTxData()[1]);
        }
      }
      break;
    case 0x2ff:
      for(int i = 0;i<8;i+=2)
      {
        CanPort* port = canPortList->getForId(CAN_ID_2FF_MSR+(i/2));
        if(port != nullptr)
        {
          txBuf_2ff.Set(i,port->GetTxData()[0]);
          txBuf_2ff.Set(i+1,port->GetTxData()[1]);
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

void CanBusMangerCall(void const * argument)
{
  bool chackSign = false;
  for(u16 count = 0;;count++)
  {
    /*****更新下行总线数据******/
    CanBusManger::Updata();

    MotorControlSystem::UpdateCanParm();

    /*******更新数据总线********/
    CanBusManger::TxBufUpdata(0x200);
    CanBusManger::TxBufUpdata(0x1ff);
    CanBusManger::TxBufUpdata(0x2ff);

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