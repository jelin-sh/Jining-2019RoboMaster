//
// Created by Administrator on 2019/5/4.
//

#include <usart.h>
#include <cstring>
#include "cmsis_os.h"
#include "CourtSystem.h"


u8 courtRxBuf;
CourtSystem::CourtRxData* courtRxData;
CourtSystem::ext_power_heat_data_t powerHeatData;

void CourtSystem::Init ()
{
  courtRxData = new CourtRxData();
  HAL_UART_Receive_IT(&COURT_UART_HANDLER,&courtRxBuf,1);
}

void CourtSystem::SerialPortRxCallBack ()
{
  HAL_UART_Receive_IT(&COURT_UART_HANDLER,&courtRxBuf,1);
  courtRxData->DecodeData(courtRxBuf);
  if(courtRxData->Ready())
  {
    u8* data = courtRxData->GetData();
    u16 cmd = courtRxData->GetCMD();
    switch (cmd)
    {
      case COURT_CMD_POWERHEATDATA:
        powerHeatData.chassis_volt = (u16)(data[1]<<8)+data[0];
        powerHeatData.chassis_current = (u16)(data[3]<<8)+data[2];
//        powerHeatData.chassis_power =
//                (float)((float)(data[7]<<24)+
//                (float)(data[6]<<16)+
//                (float)(data[5]<<8)+
//                data[4]);
        memcpy(&powerHeatData.chassis_power,data+4,4);
        powerHeatData.chassis_power_buffer = (u16)(data[9]<<8)+data[8];
        powerHeatData.shooter_heat0 = (u16)(data[11]<<8)+data[10];
        powerHeatData.shooter_heat1 = (u16)(data[13]<<8)+data[12];
        //System::pc::println("HelloWorld");
        break;
    }
    courtRxData->ClearData();
  }
}

CourtSystem::ext_power_heat_data_t* CourtSystem::GetPowerHeatData ()
{
  return &powerHeatData;
}


void CourtSystem::CourtRxData::DecodeData (u8 ch)
{

  static u8 count = 0;
  static u8* p = nullptr;

  if(ch == 0xA5)
  {
    count = 0;
    p = (u8*)&courtRxData->header;
    ready = false;
  }

  if(p != nullptr)
  {
    *p = ch;
  }

  if(count == 5)
  {
    p = frame_id;
    data_length = (u16)(header.data_lengthH<<8)+header.data_lengthL;
  }

  if(count == 7)
  {
    p = data;
    cmd = (u16)(frame_id[1]<<8)+frame_id[0];
  }

  if(count == data_length + 7)
  {
    p = frame_tail;
  }

  if(p == frame_tail+1)
  {
    p = nullptr;
    ready = true;
  }
  else
  {
    p++;
    count++;
  }
}

bool CourtSystem::CourtRxData::Ready ()
{
  return ready;
}

void CourtSystem::CourtRxData::ClearData ()
{
  memset(data,0,50);
}

u8 *CourtSystem::CourtRxData::GetData ()
{
  return data;
}

u16 CourtSystem::CourtRxData::GetCMD ()
{
  return cmd;
}
