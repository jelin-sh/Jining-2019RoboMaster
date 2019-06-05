//
// Created by Administrator on 2019/4/6.
//

#include "CanTxBuf.h"

void CanTxBuf::Set(u8 num, u8 data)
{
  buf[num] = data;
}

u8* CanTxBuf::Get()
{
  return buf;
}
