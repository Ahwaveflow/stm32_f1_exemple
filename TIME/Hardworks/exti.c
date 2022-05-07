#include "exti.h"

void EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;																								//定义结构体对象
	 __HAL_RCC_GPIOA_CLK_ENABLE();																								//使能GPIOA时钟
	
	GPIO_Initure.Pin = GPIO_PIN_4 | GPIO_PIN_5;					//选择GPIOA引脚
	GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;																			//设置GPIOA引脚的工作模式为下降沿触发
	GPIO_Initure.Pull = GPIO_PULLUP;																							//选择GPIOA引脚的上下拉方式为无上下拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; 																		//设置GPIOA的上下拉模式为上拉模式
	
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);          //初始化GPIOA
	
	HAL_NVIC_SetPriority(EXTI4_IRQn,2,0); //设置抢占优先级2，子优先级0（在stm32f1xx_hal_cortex.c的165行）
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);       //设置中断线4（在stm32f1xx_hal_cortex.c的187行）（EXTI4_IRQn在stm32f103xb.h的90行（名为IRQn_Type的结构体里））
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,1); //设置抢占优先级2，子优先级1（0的优先级最高，依次往后是1，2，3，4......）
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);       //设置中断线5（中断线5~9共用中断向量“EXTI9_5_IRQn”；10~15共用中断向量“EXTI15_10_IRQn”）
}

//中断服务函数
void EXTI4_IRQHandler(void)            //在启动文件startup_stm32f103xb.s里面有声明（名字不能自己改动）
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//调用中断处理公用函数(在stm32f1xx_hal_gpio.c的546行)
}
void EXTI9_5_IRQHandler(void)          //在启动文件startup_stm32f103xb.s里面有声明
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);//调用中断处理公用函数
}

//中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//(在stm32f1xx_hal_gpio.c的570行)
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
