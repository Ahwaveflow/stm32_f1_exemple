#include "main.h"

int main(void)
{
	HAL_Init();									 //初始化HAL库
	Stm32_Clock_Init();					 //初始化系统时钟
	delay_init();								 //32延时函数初始化
	
	Uart_Init(115200);							//串口初始化(波特率为115200)
	HAL_UART_Receive_IT(&UART_Handle,&aRxBuffer,1);				//更新接收中断
	while(1)
	{
//		printf("Hello\r\n");
//		delay_ms(500);
	}
}
