//
// Created by Administrator on 2019/4/6.
//

#include <CanBusManger.h>
#include "CanListener.h"

CanListener::CanListener(u16 portId)
{
  canPort =  CanBusManger::GetPortList()->getForId(portId);
}

u8 *CanListener::GetData()
{
  return canPort->GetRxData();
}

void CanListener::WriteData(short data)
{
  canPort->WriteData(data);
}

bool CanListener::isOnline()
{
  return canPort->isOnline();
}
