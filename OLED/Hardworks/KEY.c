#include "key.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;											//定义结构体对象
	 __HAL_RCC_GPIOA_CLK_ENABLE();											//使能GPIOA时钟
	
	GPIO_Initure.Pin = GPIO_PIN_4 | GPIO_PIN_5;					//选择GPIOA引脚
	GPIO_Initure.Mode = GPIO_MODE_INPUT;								//选择GPIOA引脚的工作模式输入模式
	GPIO_Initure.Pull = GPIO_PULLUP;										//选择GPIOA引脚的上下拉方式为上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;					//选择GPIOA引脚的工作速度为快速模式
	
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);          //初始化GPIOA
}




uint8_t KEY_Scan(void)							//按键扫描
{
	static uint8_t key_up = 1;				//按键松开标志位·
	if(key_up && (K1 == 0 || K2 == 0))
	{
		HAL_Delay(10);
		key_up = 0;
		if(K1 == 0) return K1_PRES;
		else if(K2 == 0) return K2_PRES;
	}
	else if(K1 == 1 && K2 == 1) key_up = 1;			//无按键按下
	return 0;
	
}

void KEY_Function(void)						//按键功能
{
	static uint8_t key = 0;
	key = KEY_Scan();
	switch(key)
	{
		case K1_PRES:
			led();break;  
		case K2_PRES:
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);break;  //翻转电平
		default:
			break;
	}
}
	
