//
// Created by jelin on 19-5-23.
//

#ifndef ENGINEERING_UPISLANDCONTROLUNIT_H
#define ENGINEERING_UPISLANDCONTROLUNIT_H



class UpIslandControlUnit {
public:
  static void OpenMotor();
  static void CloseMotor();
public:
  static void Init();
  static bool IsOnLand();
  static void UpdateMotorPramForCan();
  static void UpdateControlPramForRemote();
  static void UpdateControlPramForPC();
};



#endif //ENGINEERING_UPISLANDCONTROLUNIT_H
