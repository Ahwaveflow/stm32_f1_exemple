#include "main.h"

int main(void)
{
	HAL_Init();									 //��ʼ��HAL��
	Stm32_Clock_Init();					 //��ʼ��ϵͳʱ��
	delay_init();								 //32��ʱ������ʼ��
	
	Uart_Init(115200);							//���ڳ�ʼ��(������Ϊ115200)
	HAL_UART_Receive_IT(&UART_Handle,&aRxBuffer,1);				//���½����ж�
	while(1)
	{
//		printf("Hello\r\n");
//		delay_ms(500);
	}
}
