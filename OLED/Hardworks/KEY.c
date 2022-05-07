#include "key.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;											//����ṹ�����
	 __HAL_RCC_GPIOA_CLK_ENABLE();											//ʹ��GPIOAʱ��
	
	GPIO_Initure.Pin = GPIO_PIN_4 | GPIO_PIN_5;					//ѡ��GPIOA����
	GPIO_Initure.Mode = GPIO_MODE_INPUT;								//ѡ��GPIOA���ŵĹ���ģʽ����ģʽ
	GPIO_Initure.Pull = GPIO_PULLUP;										//ѡ��GPIOA���ŵ���������ʽΪ����
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;					//ѡ��GPIOA���ŵĹ����ٶ�Ϊ����ģʽ
	
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);          //��ʼ��GPIOA
}




uint8_t KEY_Scan(void)							//����ɨ��
{
	static uint8_t key_up = 1;				//�����ɿ���־λ��
	if(key_up && (K1 == 0 || K2 == 0))
	{
		HAL_Delay(10);
		key_up = 0;
		if(K1 == 0) return K1_PRES;
		else if(K2 == 0) return K2_PRES;
	}
	else if(K1 == 1 && K2 == 1) key_up = 1;			//�ް�������
	return 0;
	
}

void KEY_Function(void)						//��������
{
	static uint8_t key = 0;
	key = KEY_Scan();
	switch(key)
	{
		case K1_PRES:
			led();break;  
		case K2_PRES:
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);break;  //��ת��ƽ
		default:
			break;
	}
}
	
