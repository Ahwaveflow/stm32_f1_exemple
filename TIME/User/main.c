#include "main.h"

int main(void)
{
	HAL_Init();             //初始化HAL库
	Stm32_Clock_Init();     //初始化系统时钟
	delay_init();						//32延时函数初始化
	
	LED_Init();             //初始化LED
	TIM3_Init(8999,7999);		//定时器3初始化 定时1s		
	while(1)
	{
		
	}
}
