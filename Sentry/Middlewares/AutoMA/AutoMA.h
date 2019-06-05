//
// Created by jelin on 19-5-17.
//

#ifndef INFENTERY_AUTOMA_H
#define INFENTERY_AUTOMA_H

#include <stm32plus.h>

#define AUTOMA_PITCH_MID  240
#define AUTOMA_YAW_MID    320
class AutoMA {
public:
  static float CalcPithAngle();
  static float CalcYawAngle();
  static void Init();
};



#endif //INFENTERY_AUTOMA_H
