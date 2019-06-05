//
// Created by AdminipcTxBufator on 2019/2/25.
//
#include "System.h"
#include "stdarg.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef *hUartHandler;
u8 pcTxBuf[200] = {0};
void System::pc::setHandler(UART_HandleTypeDef *_hUartHandler) {
  hUartHandler = _hUartHandler;
}

int System::pc::printf(char *fmt, ...) {
  if (hUartHandler == NULL) {
    return 0;
  }
  memset(pcTxBuf,0,200);
  va_list ap;
  va_start(ap, fmt);
  vsprintf(reinterpret_cast<char *>(pcTxBuf), fmt, ap);
  va_end(ap);
  if (HAL_UART_Transmit(
          hUartHandler, (uint8_t *)pcTxBuf,
          (uint16_t)strlen(reinterpret_cast<const char *>(pcTxBuf)),
          100) == HAL_OK) {
    return 1;
  }
  return 0;
}

int System::pc::println(char *fmt, ...) {
  if (hUartHandler == NULL) {
    return 0;
  }
  memset(pcTxBuf,0,200);
  va_list ap;
  va_start(ap, fmt);
  vsprintf(reinterpret_cast<char *>(pcTxBuf), fmt, ap);
  va_end(ap);
  strcat(reinterpret_cast<char *>(pcTxBuf), "\r\n");
  if (HAL_UART_Transmit(
          hUartHandler, (uint8_t *)pcTxBuf,
          (uint16_t)strlen(reinterpret_cast<const char *>(pcTxBuf)),
          10) == HAL_OK) {
    return 1;
  }
  return 0;
}

int System::pc::log(char *log, char *fmt, ...) {
  if (hUartHandler == NULL) {
    return 0;
  }
  memset(pcTxBuf,0,200);
  va_list ap;
  va_start(ap, fmt);
  vsprintf(reinterpret_cast<char *>(pcTxBuf), fmt, ap);
  va_end(ap);
  strcat(reinterpret_cast<char *>(pcTxBuf), "\r\n");
  HAL_UART_Transmit(hUartHandler, (uint8_t *)"[", 1, 20);
  HAL_UART_Transmit(hUartHandler, (uint8_t *)log, (uint16_t)strlen(log), 20);
  HAL_UART_Transmit(hUartHandler, (uint8_t *)"]:", 2, 20);
  if (HAL_UART_Transmit(
          hUartHandler, (uint8_t *)pcTxBuf,
          (uint16_t)strlen(reinterpret_cast<const char *>(pcTxBuf)),
          20) == HAL_OK) {
    return 1;
  }
  return 0;
}
