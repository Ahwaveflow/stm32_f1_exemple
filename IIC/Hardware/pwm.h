#ifndef __PWM_H
#define __PWM_H

#include "stm32f1xx_hal.h"
#include "main.h"

void PWM_Init(uint32_t Arr1,uint32_t Pcs1);
void PWM_Compare(uint32_t compare);
void LED_indicator(void);

#endif
