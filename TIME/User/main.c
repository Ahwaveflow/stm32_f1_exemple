#include "main.h"

int main(void)
{
	HAL_Init();             //��ʼ��HAL��
	Stm32_Clock_Init();     //��ʼ��ϵͳʱ��
	delay_init();						//32��ʱ������ʼ��
	
	LED_Init();             //��ʼ��LED
	TIM3_Init(8999,7999);		//��ʱ��3��ʼ�� ��ʱ1s		
	while(1)
	{
		
	}
}
