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
	return KEY_NULL;
	
}

void KEY_Function(void)						//按键功能
{
	static uint8_t key = 0;
	const char text_buffer[] = "AT24C02_TEXT";		//更换这里的字符串就可以更改写入的数据
	#define text_size sizeof(text_buffer)			//字符串的大小
	#define address 0								//写入的地址
	u8 data_temp[text_size];						//存储读取的数据

	key = KEY_Scan();
	switch(key)
	{
		case KEY_NULL:
			{
				AT24CXX_Write(address,(uint8_t *)text_buffer,text_size);//写入数据 
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
				OLED_ShowString(0,2,(uint8_t *)data_temp);//显示读取的数据 
				break; 
			}
		case K2_PRES:
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);break;  //翻转电平
		default:
			break;
	}
}
	
