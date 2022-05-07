#include "ad.h"

ADC_HandleTypeDef ADC_Handle;			//ADC句柄

void ADC_Init(void)
{
		RCC_PeriphCLKInitTypeDef ADC_CLKInit;
		
		ADC_CLKInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;   //ADC外设时钟
		ADC_CLKInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;			//分频因子6时钟  72M/6=12MHz
		HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);								//设置ADC时钟
		
		ADC_Handle.Instance = ADC1;
		ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;				//右对齐
		ADC_Handle.Init.ScanConvMode =  ADC_SCAN_DISABLE;				//非扫描模式
		ADC_Handle.Init.ContinuousConvMode = ENABLE;						//连续转换
		ADC_Handle.Init.DiscontinuousConvMode = DISABLE;				//禁止不连续采样模式
		ADC_Handle.Init.NbrOfConversion = 1;										//1个转换在规则序列中 也就是只转换规则序列1
		ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;	//软件触发
		
		HAL_ADC_Init(&ADC_Handle); 				//ADC初始化
		HAL_ADCEx_Calibration_Start(&ADC_Handle);		//校准ADC
	}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
		GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();						//开启GPIOA时钟
	
    GPIO_Initure.Pin = GPIO_PIN_0;
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;			//模拟输入
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_adc(u32 ch)
{
	ADC_ChannelConfTypeDef ADC1_channel;
	
	ADC1_channel.Channel = ch;					  //通道
	ADC1_channel.Rank = ADC_REGULAR_RANK_1;				//第1个序列，序列1
	ADC1_channel.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;		 //采样时间 
	HAL_ADC_ConfigChannel(&ADC_Handle,&ADC1_channel);		//通道配置
	
	HAL_ADC_Start(&ADC_Handle);						 //开启ADC
	HAL_ADC_PollForConversion(&ADC_Handle,10);		//轮询转换
	
	return (u16)HAL_ADC_GetValue(&ADC_Handle);	 //返回最近一次ADC1规则组的转换结果
}

//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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





