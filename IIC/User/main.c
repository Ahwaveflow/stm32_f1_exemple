#include "main.h"

int main(void)
{
	
	HAL_Init();							//初始化HAL库
	Stm32_Clock_Init();			//初始化系统时钟
	delay_init();						//初始化延时函数
	
	OLED_Init();
	OLED_Clear();
	Key_Init();
	AT24CXX_Init();		//初始化IIC
	
	OLED_ShowCHinese(40,0,89);//显示中文字 '孙'
	OLED_ShowCHinese(56,0,90);//显示中文字 '佳'
	OLED_ShowCHinese(72,0,91);//显示中文字 '鹏'
		
	while(1)
	{
		KEY_Function();
	}
}

