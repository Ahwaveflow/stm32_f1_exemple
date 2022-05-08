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
	return KEY_NULL;
	
}

void KEY_Function(void)						//��������
{
	static uint8_t key = 0;
	const char text_buffer[] = "AT24C02_TEXT";		//����������ַ����Ϳ��Ը���д�������
	#define text_size sizeof(text_buffer)			//�ַ����Ĵ�С
	#define address 0								//д��ĵ�ַ
	u8 data_temp[text_size];						//�洢��ȡ������

	key = KEY_Scan();
	switch(key)
	{
		case KEY_NULL:
			{
				AT24CXX_Write(address,(uint8_t *)text_buffer,text_size);//д������ 
				OLED_ShowString(0,0,(uint8_t *)"writting..."); 
				delay_ms(800); 
				OLED_Clear(); 
				delay_ms(50); 
				OLED_ShowString(0,0,(uint8_t *)"write is ok");
				break;
			}
		case K1_PRES:
			{
				AT24CXX_Read(address,(uint8_t *)data_temp,text_size); 
				OLED_ShowString(0,0,(uint8_t *)"reading..."); 
				delay_ms(800); 
				OLED_Clear(); 
				delay_ms(50); 
				OLED_ShowString(0,0,(uint8_t *)"read is ok"); 
				delay_ms(800); 
				OLED_Clear(); 
				OLED_ShowString(0,2,(uint8_t *)data_temp);//��ʾ��ȡ������ 
				break; 
			}
		case K2_PRES:
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);break;  //��ת��ƽ
		default:
			break;
	}
}
	
