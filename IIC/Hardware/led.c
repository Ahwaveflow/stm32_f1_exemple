#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;																								//定义结构体对象
	 __HAL_RCC_GPIOA_CLK_ENABLE();																								//使能GPIOA时钟
	
	GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;					//选择GPIOA引脚
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;																			//选择GPIOA引脚的工作模式为推挽输出
	GPIO_Initure.Pull = GPIO_NOPULL;																							//选择GPIOA引脚的上下拉方式为无上下拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; 																		//选择GPIOA引脚的工作速度为快速模式
	
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);																						//初始化GPIOA
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,GPIO_PIN_SET);	//写GPIOA的0,1,2,3这几个引脚的状态为高电平
}



void led(void)
{
	  int k,j;
	
		for(k=0;k<4;k++)
		{
			delay_ms(500);
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
			
		}
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_2,GPIO_PIN_RESET);
		
		for(j=0;j<5;j++)
		{
			delay_ms(500);
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
			
		}
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_2,GPIO_PIN_RESET);
}
