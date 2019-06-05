//
// Created by Administrator on 2019/4/6.
//

#include <cstring>
#include "CanPort.h"

CanPort::CanPort(u16 portId)
{
  id = portId;
  ClearRxData();
  ClearTxData();
}

u8 *CanPort::GetRxData()
{
  return rxData;
}
u8 *CanPort::GetTxData()
{
  return txData;
}
u16 CanPort::GetID()
{
  return id;
}
void CanPort::WriteData(short val)
{
  txData[0] = val>>8;
  txData[1] = val&0xff;
}

void CanPort::ReceiveData(u8 *buf)
{
  online = true;
  for(int i = 0;i<8;i++)
  {
    rxData[i] = buf[i];
  }
}

void CanPort::ClearTxData()
{
  txData[0] = 0;
  txData[1] = 0;
}
void CanPort::ClearRxData()
{
  for(int i = 0;i<8;i++)
  {
    rxData[i] = 0;
  }
}
void CanPort::printRxData()
{
  System::pc::printf("[Port:0x%02x] ",id);
  for(int i = 0;i<8;i++)
  {
    System::pc::printf("0x%02x ",rxData[i]);
  }
  System::pc::printf("\r\n");
}
void CanPort::printTxData()
{
  System::pc::printf("[Port:0x%02x] ",id);
  for(int i = 0;i<2;i++)
  {
    System::pc::printf("0x%02x ",txData[i]);
  }
  System::pc::printf("\r\n");
}

bool CanPort::isOnline()
{
  return online;
}




