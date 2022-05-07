#include "main.h"

int main(void)
{
	HAL_Init();							//��ʼ��HAL��
	Stm32_Clock_Init();			//��ʼ��ϵͳʱ��
	delay_init();						//��ʼ����ʱ����
	
	OLED_Init();
	OLED_Clear();
	
	while(1)
	{
//		Display_bmp();
		OLED_ShowCHinese(40,0,89);//��ʾ������
		OLED_ShowCHinese(56,0,90);//��ʾ������
		OLED_ShowCHinese(72,0,91);//��ʾ������
		
		OLED_ShowString(56,2,(uint8_t *)"SB");//��ʾ�ַ��� 
	}
}
 