#include "main.h"

int main(void)
{
	
	HAL_Init();							//��ʼ��HAL��
	Stm32_Clock_Init();			//��ʼ��ϵͳʱ��
	delay_init();						//��ʼ����ʱ����
	
	OLED_Init();
	OLED_Clear();
	Key_Init();
	AT24CXX_Init();		//��ʼ��IIC
	
	OLED_ShowCHinese(40,0,89);//��ʾ������ '��'
	OLED_ShowCHinese(56,0,90);//��ʾ������ '��'
	OLED_ShowCHinese(72,0,91);//��ʾ������ '��'
		
	while(1)
	{
		KEY_Function();
	}
}

