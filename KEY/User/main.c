#include "main.h"

int main(void)
{
	HAL_Init();             //��ʼ��HAL��
	Stm32_Clock_Init();     //��ʼ��ϵͳʱ��
	delay_init();           //��ʼ����ʱ
	
	LED_Init();             //��ʼ��LED
	Key_Init();             //��ʼ������
	
	while(1)
	{
		KEY_Function();
	}
}
