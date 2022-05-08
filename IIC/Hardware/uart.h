#ifndef __UART_H
#define __UART_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

extern UART_HandleTypeDef UART_Handle;	
extern uint8_t aRxBuffer;


void Uart_Init(uint32_t Bound);


#endif
