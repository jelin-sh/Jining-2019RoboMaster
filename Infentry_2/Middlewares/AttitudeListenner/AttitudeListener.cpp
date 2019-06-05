//
// Created by Administrator on 2019/3/10.
//

#include "AttitudeListener.h"
#include "cmsis_os.h"
#include <cmath>

float yaw;
float pitch;
float roll;
bool busy = false;
osThreadId attitudeThreadID;

void AttitudeListenerTask(void const * argument);

void AttitudeListener::updata()
{
  busy = true;
  mpu_get_data();
  imu_ahrs_update();
  imu_attitude_update();
  busy = false;
}

void AttitudeListener::Init()
{
  mpu_device_init();
  init_quaternion();
  osThreadDef(attitudeThread,AttitudeListenerTask,osPriorityNormal,0,512);
  attitudeThreadID = osThreadCreate(osThread(attitudeThread),NULL);
}

imu_t* AttitudeListener::getImu()
{
  if(IsBusy())
  {
    return nullptr;
  }
  return &imu;
}

bool AttitudeListener::IsBusy ()
{
  return busy;
}

void AttitudeListenerTask(void const * argument)
{
  for(;;)
  {
    AttitudeListener::updata();
    int wz = (int)(AttitudeListener::getImu()->wz*1000);
//    int wy = (int)(AttitudeListener::getImu().wy*1000);
//    int wx = (int)(AttitudeListener::getImu().wx*1000);
    if(fabs(wz) > 10)
    {
//      System::pc::println("speed:%d ",wz); /*YAW���ٶ�*/
    }
//    if(fabs(wx) > 100)
//    {
//      System::pc::println("PITCH WX*1000:%d",wx); /*PITCH���ٶ�*/
//    }
    osDelay(1);
  }
}
