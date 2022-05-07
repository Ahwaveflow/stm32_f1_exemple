#include "pwm.h"

TIM_HandleTypeDef PWM_Handle;									 //���嶨ʱ���ľ������stm32f1xx_hal_tim.h��381�У����ýṹ�����滹Ƕ���˽ṹ�壩�����ȫ�֣����������жϷ������ᱨ��
TIM_OC_InitTypeDef PWM_channel_Handle;

void PWM_Init(uint32_t Arr1,uint32_t Pcs1)
{
	PWM_Handle.Instance = TIM2;										 //���ö�ʱ��Ϊͨ�ö�ʱ��2
// 	PWM_Handle.Channel = TIM_CHANNEL_1; 	
	PWM_Handle.Init.Period = Arr1;										//�����Զ���װ��ֵ
	PWM_Handle.Init.Prescaler = Pcs1;								//����Ԥ��Ƶϵ��
	PWM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;		//���嶨ʱ���ļ���ģʽΪ���ϼ���
	PWM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 	//ʱ�ӷ�Ƶ����(1��Ƶ)
	HAL_TIM_PWM_Init(&PWM_Handle);
	
	PWM_channel_Handle.OCMode = TIM_OCMODE_PWM1; 	  //ģʽһ������ֵС�ڱȽ�ֵΪ��Ч��ƽ   ģʽ��������ֵС�ڱȽ�ֵΪ��Ч��ƽ
	PWM_channel_Handle.Pulse = 0;
	PWM_channel_Handle.OCPolarity = TIM_OCPOLARITY_LOW;		//��������(��Ч��ƽ)Ϊ�͵�ƽ
	
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_1); 	//����ͨ��1
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_1);						 //������ʱ��2ͨ��1
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_2); 	//����ͨ��2
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_2);						 //������ʱ��2ͨ��2
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_3); 	//����ͨ��3
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_3);						 //������ʱ��2ͨ��3
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_4); 	//����ͨ��4
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_4);						 //������ʱ��2ͨ��4
}  


//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		GPIO_InitTypeDef GPIO_Handle;
			
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
			
		GPIO_Handle.Mode = GPIO_MODE_AF_PP;				//�����������
		GPIO_Handle.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
		GPIO_Handle.Pull = GPIO_PULLUP; 					//��������
		GPIO_Handle.Speed = GPIO_SPEED_FREQ_HIGH; //����
			
		HAL_GPIO_Init(GPIOA,&GPIO_Handle);
	}
}


//����TIMͨ����ռ�ձ�
//compare:�Ƚ�ֵ
void PWM_Compare(uint32_t compare)
{
	TIM2->CCR1 = compare;
	TIM2->CCR2 = compare;
	TIM2->CCR3 = compare;
	TIM2->CCR4 = compare;
}


void LED_indicator(void)    //������
{
	uint16_t PWM_Value = 0;
	uint8_t i;
	
	for(i=0;i<100;i++)
		{	
			PWM_Compare(PWM_Value);
			PWM_Value += 1;
			delay_ms(20);
		}
		for(i=0;i<100;i++)
		{
			PWM_Value -= 1;
			PWM_Compare(PWM_Value);
			delay_ms(20);
		}
}
