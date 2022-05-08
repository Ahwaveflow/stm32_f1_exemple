#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;																								//����ṹ�����
	 __HAL_RCC_GPIOA_CLK_ENABLE();																								//ʹ��GPIOAʱ��
	
	GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;					//ѡ��GPIOA����
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;																			//ѡ��GPIOA���ŵĹ���ģʽΪ�������
	GPIO_Initure.Pull = GPIO_NOPULL;																							//ѡ��GPIOA���ŵ���������ʽΪ��������
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; 																		//ѡ��GPIOA���ŵĹ����ٶ�Ϊ����ģʽ
	
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);																						//��ʼ��GPIOA
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,GPIO_PIN_SET);	//дGPIOA��0,1,2,3�⼸�����ŵ�״̬Ϊ�ߵ�ƽ
}



void led(void)
{
	  int k,j;
	
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
