//
// Created by jelin on 19-5-24.
//

#ifndef ENGINEERING_TAKESHOTUNIT_H
#define ENGINEERING_TAKESHOTUNIT_H

class TakeShotUnit {
public:
  static void Init();
  static void UpdateMotorParmForCan();
  static void UpdateControlPramForRemote();
  static void UpdateControlPramForPC();

  static void AutoTakeShot();
  static void ArmUp();
  static void ArmDown();
  static void ArmStay();
  static void ArmOpen();
  static void ArmClose();
  static void ArmOut();
  static void ArmBack();
  static void ArmGetUp();
  static void ArmGetDown();
  static void ArmTakeOn();
  static void ArmTakeOff();

  static bool IsTakeMode();
};



#endif //ENGINEERING_TAKESHOTUNIT_H
