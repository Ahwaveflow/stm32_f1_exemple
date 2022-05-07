#include "pwm.h"

TIM_HandleTypeDef PWM_Handle;									 //定义定时器的句柄（在stm32f1xx_hal_tim.h的381行）（该结构体里面还嵌套了结构体）定义成全局，否则后面的中断服务函数会报错
TIM_OC_InitTypeDef PWM_channel_Handle;

void PWM_Init(uint32_t Arr1,uint32_t Pcs1)
{
	PWM_Handle.Instance = TIM2;										 //设置定时器为通用定时器2
// 	PWM_Handle.Channel = TIM_CHANNEL_1; 	
	PWM_Handle.Init.Period = Arr1;										//设置自动重装载值
	PWM_Handle.Init.Prescaler = Pcs1;								//设置预分频系数
	PWM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;		//定义定时器的计数模式为向上计数
	PWM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 	//时钟分频因子(1分频)
	HAL_TIM_PWM_Init(&PWM_Handle);
	
	PWM_channel_Handle.OCMode = TIM_OCMODE_PWM1; 	  //模式一：计数值小于比较值为有效电平   模式二：计数值小于比较值为无效电平
	PWM_channel_Handle.Pulse = 0;
	PWM_channel_Handle.OCPolarity = TIM_OCPOLARITY_LOW;		//极性配置(有效电平)为低电平
	
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_1); 	//配置通道1
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_1);						 //开启定时器2通道1
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_2); 	//配置通道2
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_2);						 //开启定时器2通道2
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_3); 	//配置通道3
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_3);						 //开启定时器2通道3
	
	HAL_TIM_PWM_ConfigChannel(&PWM_Handle,&PWM_channel_Handle,TIM_CHANNEL_4); 	//配置通道4
	HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_4);						 //开启定时器2通道4
}  


//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		GPIO_InitTypeDef GPIO_Handle;
			
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
			
		GPIO_Handle.Mode = GPIO_MODE_AF_PP;				//复用推挽输出
		GPIO_Handle.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
		GPIO_Handle.Pull = GPIO_PULLUP; 					//上拉输入
		GPIO_Handle.Speed = GPIO_SPEED_FREQ_HIGH; //高速
			
		HAL_GPIO_Init(GPIOA,&GPIO_Handle);
	}
}


//设置TIM通道的占空比
//compare:比较值
void PWM_Compare(uint32_t compare)
{
	TIM2->CCR1 = compare;
	TIM2->CCR2 = compare;
	TIM2->CCR3 = compare;
	TIM2->CCR4 = compare;
}


void LED_indicator(void)    //呼吸灯
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
