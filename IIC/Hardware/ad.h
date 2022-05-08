#ifndef __AD_H
#define __AD_H

#include "stm32f1xx_hal.h"
#include "main.h"

void ADC_Init(void);
u16 Get_adc(u32 ch);
u16 Get_ADC_Average(u32 ch,u8 times);

#endif
