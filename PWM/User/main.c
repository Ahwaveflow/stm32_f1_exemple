#include "main.h"

int main(void)
{
	HAL_Init();             //初始化HAL库
	Stm32_Clock_Init();     //初始化系统时钟
	delay_init();						//32延时函数初始化
	
//	LED_Init();             //初始化LED
//	TIM3_Init(9999,3599);
	PWM_Init(99,7199);
	while(1)
	{
		LED_indicator();
	}
}
