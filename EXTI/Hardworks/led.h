#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"
#include "main.h"

#define LED1 PAout(0)
#define LED2 PAout(1)
#define LED3 PAout(2)
#define LED4 PAout(3)

void LED_Init(void);
void led(void);


#endif
