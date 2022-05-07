#include "main.h"


int k,j;

int main(void)
{
	
//	static int j = 0;
	
	HAL_Init();             //��ʼ��HAL��
	Stm32_Clock_Init();     //��ʼ��ϵͳʱ��
	delay_init();						//32��ʱ������ʼ��
	
	LED_Init();             //��ʼ��LED
	
//	delay_ms(1000);
	while(1)
	{
//		led();
		for(k=0;k<4;k++)
		{
			delay_ms(500);
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
			
		}
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_2,GPIO_PIN_RESET);
		
		for(j=0;j<5;j++)
		{
			delay_ms(500);
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
			
		}
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_2,GPIO_PIN_RESET);
	}
}
