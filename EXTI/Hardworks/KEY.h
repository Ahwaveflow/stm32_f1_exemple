#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"
#include "main.h"

#define K1 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)
#define K2 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)

#define K1_PRES 1
#define K2_PRES 2

void Key_Init(void);

uint8_t KEY_Scan(void);
void KEY_Function(void);

#endif


