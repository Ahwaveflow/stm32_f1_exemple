#include "time.h"

/*如未添加定时器相关的头文件请添加stm32f1xx_hal_tim.c和stm32f1xx_hal_tim_ex.c和stm32f1xx_hal_dma.c*/
TIM_HandleTypeDef TIM_Handle;					//定义定时器的句柄（在stm32f1xx_hal_tim.h的381行）（该结构体里面还嵌套了结构体）定义成全局，否则后面的中断服务函数会报错

void TIM3_Init(uint32_t Arr,uint32_t Pcs)
{
	TIM_Handle.Instance = TIM3;								 							//设置定时器为通用定时器3
	TIM_Handle.Init.Period = Arr;							 							//自动重装载值
	TIM_Handle.Init.Prescaler = Pcs;					 							//预分频系数      	Tout = [(Arr + 1) * (Pcs + 1)] / fr     72MHz
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;				//向上计数模式
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;     //分频因子1

	HAL_TIM_Base_Init(&TIM_Handle);													//初始化定时器3
	HAL_TIM_Base_Start_IT(&TIM_Handle);											//开启定时器3中断
}
 


//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)							//MSP函数一般用来使能，（回调函数 __weak）
{
	if(htim->Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();							//使能TIM3时钟
		
		HAL_NVIC_SetPriority(TIM3_IRQn, 1,3);			//设置抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);						//开启定时器2通道
	}
}



//中断服务函数
void TIM3_IRQHandler()					//在启动文件startup_stm32f103xb.s里面有声明（名字不能自己改动）
{
	HAL_TIM_IRQHandler(&TIM_Handle);					//定时器中断处理通用函数
}


//回调函数，定时器中断服务函数调用
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
