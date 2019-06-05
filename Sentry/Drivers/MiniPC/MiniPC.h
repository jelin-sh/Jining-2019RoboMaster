//
// Created by jelin on 19-5-16.
//

#ifndef INFENTERY_MINIPC_H
#define INFENTERY_MINIPC_H

#include <stm32plus.h>

class MiniPC {
public:
  static void Init();
  static void Start();
  static void MiniPCRxCall();
  static u16 GetX();
  static u16 GetY();
  static bool isOnline();
  static void ClearX();
  static void ClearY();
};



#endif //INFENTERY_MINIPC_H
