#include "ad.h"

ADC_HandleTypeDef ADC_Handle;			//ADC���

void ADC_Init(void)
{
		RCC_PeriphCLKInitTypeDef ADC_CLKInit;
		
		ADC_CLKInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;   //ADC����ʱ��
		ADC_CLKInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;			//��Ƶ����6ʱ��  72M/6=12MHz
		HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);								//����ADCʱ��
		
		ADC_Handle.Instance = ADC1;
		ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;				//�Ҷ���
		ADC_Handle.Init.ScanConvMode =  ADC_SCAN_DISABLE;				//��ɨ��ģʽ
		ADC_Handle.Init.ContinuousConvMode = ENABLE;						//����ת��
		ADC_Handle.Init.DiscontinuousConvMode = DISABLE;				//��ֹ����������ģʽ
		ADC_Handle.Init.NbrOfConversion = 1;										//1��ת���ڹ��������� Ҳ����ֻת����������1
		ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;	//�������
		
		HAL_ADC_Init(&ADC_Handle); 				//ADC��ʼ��
		HAL_ADCEx_Calibration_Start(&ADC_Handle);		//У׼ADC
	}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
		GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();						//����GPIOAʱ��
	
    GPIO_Initure.Pin = GPIO_PIN_0;
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;			//ģ������
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_adc(u32 ch)
{
	ADC_ChannelConfTypeDef ADC1_channel;
	
	ADC1_channel.Channel = ch;					  //ͨ��
	ADC1_channel.Rank = ADC_REGULAR_RANK_1;				//��1�����У�����1
	ADC1_channel.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;		 //����ʱ�� 
	HAL_ADC_ConfigChannel(&ADC_Handle,&ADC1_channel);		//ͨ������
	
	HAL_ADC_Start(&ADC_Handle);						 //����ADC
	HAL_ADC_PollForConversion(&ADC_Handle,10);		//��ѯת��
	
	return (u16)HAL_ADC_GetValue(&ADC_Handle);	 //�������һ��ADC1�������ת�����
}

//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_ADC_Average(u32 ch,u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0;t < times;t++)
	{
		temp_val += Get_adc(ch);
		delay_ms(5);
	}
	return (temp_val/times);
}





