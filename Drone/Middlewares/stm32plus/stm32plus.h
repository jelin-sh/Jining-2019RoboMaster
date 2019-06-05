//
// Created by Administrator on 2019/2/20.
//

#ifndef STM32_PLUS_STM32PLUS_H
#define STM32_PLUS_STM32PLUS_H

#include "stm32f4xx_hal.h"
#include "System.h"
#include "Port.h"
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 BYTE;
typedef u16 WORD;
typedef u32 DWORD;

typedef enum
{
    STM32PLUS_OK = 0,
    STM32PLUS_ERR,

}STM32Plus_Status;

#endif //STM32_PLUS_STM32PLUS_H
