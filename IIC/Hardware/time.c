#include "time.h"

/*��δ��Ӷ�ʱ����ص�ͷ�ļ������stm32f1xx_hal_tim.c��stm32f1xx_hal_tim_ex.c��stm32f1xx_hal_dma.c*/
TIM_HandleTypeDef TIM_Handle;					//���嶨ʱ���ľ������stm32f1xx_hal_tim.h��381�У����ýṹ�����滹Ƕ���˽ṹ�壩�����ȫ�֣����������жϷ������ᱨ��

void TIM3_Init(uint32_t Arr,uint32_t Pcs)
{
	TIM_Handle.Instance = TIM3;								 							//���ö�ʱ��Ϊͨ�ö�ʱ��3
	TIM_Handle.Init.Period = Arr;							 							//�Զ���װ��ֵ
	TIM_Handle.Init.Prescaler = Pcs;					 							//Ԥ��Ƶϵ��      	Tout = [(Arr + 1) * (Pcs + 1)] / fr     72MHz
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;				//���ϼ���ģʽ
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;     //��Ƶ����1

	HAL_TIM_Base_Init(&TIM_Handle);													//��ʼ����ʱ��3
	HAL_TIM_Base_Start_IT(&TIM_Handle);											//������ʱ��3�ж�
}
 


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)							//MSP����һ������ʹ�ܣ����ص����� __weak��
{
	if(htim->Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();							//ʹ��TIM3ʱ��
		
		HAL_NVIC_SetPriority(TIM3_IRQn, 1,3);			//������ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);						//������ʱ��2ͨ��
	}
}



//�жϷ�����
void TIM3_IRQHandler()					//�������ļ�startup_stm32f103xb.s���������������ֲ����Լ��Ķ���
{
	HAL_TIM_IRQHandler(&TIM_Handle);					//��ʱ���жϴ���ͨ�ú���
}


//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t count = 0;
	if(htim->Instance == TIM3)
	{
		count++;
		if(count > 1)
		{
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0 | GPIO_PIN_2);
		}
	}
}
