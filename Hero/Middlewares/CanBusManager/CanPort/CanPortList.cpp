//
// Created by Administrator on 2019/4/6.
//

#include "CanPortList.h"


CanPort *CanPortList::getForId(u16 id)
{
  for(int i = 0;i<length();i++)
  {
    if(get(i)->GetID() == id)
    {
      return get(i);
    }
  }
  return nullptr;
}
