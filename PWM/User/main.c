#include "main.h"

int main(void)
{
	HAL_Init();             //��ʼ��HAL��
	Stm32_Clock_Init();     //��ʼ��ϵͳʱ��
	delay_init();						//32��ʱ������ʼ��
	
//	LED_Init();             //��ʼ��LED
//	TIM3_Init(9999,3599);
	PWM_Init(99,7199);
	while(1)
	{
		LED_indicator();
	}
}
