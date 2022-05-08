#include "exti.h"

void EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;																								//����ṹ�����
	 __HAL_RCC_GPIOA_CLK_ENABLE();																								//ʹ��GPIOAʱ��
	
	GPIO_Initure.Pin = GPIO_PIN_4 | GPIO_PIN_5;					//ѡ��GPIOA����
	GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;																			//����GPIOA���ŵĹ���ģʽΪ�½��ش���
	GPIO_Initure.Pull = GPIO_PULLUP;																							//ѡ��GPIOA���ŵ���������ʽΪ��������
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; 																		//����GPIOA��������ģʽΪ����ģʽ
	
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);          //��ʼ��GPIOA
	
	HAL_NVIC_SetPriority(EXTI4_IRQn,2,0); //������ռ���ȼ�2�������ȼ�0����stm32f1xx_hal_cortex.c��165�У�
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);       //�����ж���4����stm32f1xx_hal_cortex.c��187�У���EXTI4_IRQn��stm32f103xb.h��90�У���ΪIRQn_Type�Ľṹ�����
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,1); //������ռ���ȼ�2�������ȼ�1��0�����ȼ���ߣ�����������1��2��3��4......��
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);       //�����ж���5���ж���5~9�����ж�������EXTI9_5_IRQn����10~15�����ж�������EXTI15_10_IRQn����
}

//�жϷ�����
void EXTI4_IRQHandler(void)            //�������ļ�startup_stm32f103xb.s���������������ֲ����Լ��Ķ���
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//�����жϴ����ú���(��stm32f1xx_hal_gpio.c��546��)
}
void EXTI9_5_IRQHandler(void)          //�������ļ�startup_stm32f103xb.s����������
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);//�����жϴ����ú���
}

//�жϻص�����
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//(��stm32f1xx_hal_gpio.c��570��)
{
	delay_ms(100);
	switch(GPIO_Pin)
	{
		case GPIO_PIN_4:
			delay_ms(10);
			if(K1 == 0)
				LED1 = !LED1 ;
			break;
			
		case GPIO_PIN_5:
			delay_ms(10);
			if(K2 == 0)
				LED2 = !LED2;
			break;		
	}
}
