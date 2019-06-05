//
// Created by Administrator on 2019/4/24.
//

#include "string.h"
#include "BlueTooth.h"
#include "cmsis_os.h"

u8* blueRxBuf = nullptr;
/*
 * rxStatus:
 * [7:6] 获取完成标识符
 * [5:0] 长度 最大128
 * **/
u8 rxStatus = 0x00;
bool rxR = false;
class BlueUart:public Uart
{
public:
    BlueUart(UART_HandleTypeDef *_uartHandler) : Uart(_uartHandler)
    {
      /*None*/
    }

public:
    void openReceiveIT() override
    {
      u8 buf;
      HAL_UART_Receive_IT(getHandler(),&buf,1);
    }

    void onRXData(u8 ch) override
    {
      if(blueRxBuf == nullptr)
      {
        //BlueTooth没有初始化
        return;
      }
      if(rxStatus&0x80)
      {
        /*获取完成*/
        return;
      }
      if(rxR && ch == '\n')
      {
        blueRxBuf[rxStatus] = 0;
        rxStatus--;
        rxStatus &= 0x7f;
        /**接收完一行**/
        return;
      }
      rxR = ch == '\r';
      blueRxBuf[rxStatus] = ch;
      rxStatus++;
    }
};

BlueUart* blueUart = nullptr;

void BlueTooth::Init(UART_HandleTypeDef uartHandleTypeDef)
{
  blueUart = new BlueUart(&uartHandleTypeDef);
  blueRxBuf = (u8*)pvPortMalloc(BLUETOOTH_RX_SIZE);
  memset(blueRxBuf,0,BLUETOOTH_RX_SIZE);
  blueUart->openReceiveIT();
}

bool BlueTooth::isRxData()
{
  return (bool)rxStatus&0x80; //取出完成标识符
}

void BlueTooth::GetData(u8 *buf)
{
  memcpy(buf,blueRxBuf,BLUETOOTH_RX_SIZE);
  memset(blueRxBuf,0,BLUETOOTH_RX_SIZE);
  rxStatus = 0;
}

