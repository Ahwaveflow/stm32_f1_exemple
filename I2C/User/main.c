#include "main.h"

int main(void)
{
	u16 temp;
	char str_buff[100];
	
	HAL_Init();							//初始化HAL库
	Stm32_Clock_Init();			//初始化系统时钟
	delay_init();						//初始化延时函数
	
	OLED_Init();
	OLED_Clear();
	ADC_Init();
	Uart_Init(115200);
	HAL_UART_Receive_IT(&UART_Handle,&aRxBuffer,1);
	
	printf("Test is OK!\r\n");
	
	OLED_ShowCHinese(40,0,89);//显示中文字
	OLED_ShowCHinese(56,0,90);//显示中文字
	OLED_ShowCHinese(72,0,91);//显示中文字
		
	OLED_ShowString(56,2,(uint8_t *)"SB");//显示字符串 
	while(1)
	{
		temp = Get_ADC_Average(0,20);
		printf("value: %d\r\n",temp);
		printf("电压: %.2f V \r\n",temp*(3.3/4096));
		sprintf(str_buff,"Voltage: %.2f V",temp*(3.3/4096));
		OLED_ShowString(0,4,(uint8_t*)str_buff);			//OLED显示电压
		delay_ms(500);
	}
}
 