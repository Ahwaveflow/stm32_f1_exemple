#include "main.h"

int main(void)
{
	HAL_Init();             //初始化HAL库
	Stm32_Clock_Init();     //初始化系统时钟
	delay_init();           //初始化延时
	
	LED_Init();             //初始化LED
	EXTI_Init();
	while(1)
	{
		
	}
}
