#include "main.h"

int main(void)
{
	HAL_Init();							//初始化HAL库
	Stm32_Clock_Init();			//初始化系统时钟
	delay_init();						//初始化延时函数
	
	OLED_Init();
	OLED_Clear();
	
	while(1)
	{
//		Display_bmp();
		OLED_ShowCHinese(40,0,89);//显示中文字
		OLED_ShowCHinese(56,0,90);//显示中文字
		OLED_ShowCHinese(72,0,91);//显示中文字
		
		OLED_ShowString(56,2,(uint8_t *)"SB");//显示字符串 
	}
}
 