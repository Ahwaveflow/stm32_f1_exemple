#include "uart.h"

uint8_t aRxBuffer;								//接收中断缓冲
uint8_t Uart1_RxBuff[256];				//接收缓冲
uint8_t Uart1_cnt = 0;						//接收缓冲计数
uint8_t cAlmStr[] = "数据溢出(大于256)\r\n";

UART_HandleTypeDef UART_Handle;				//串口句柄

void Uart_Init(uint32_t Bound)
{
		UART_Handle.Instance = USART1;												//串口1
		UART_Handle.Init.BaudRate = Bound;										//设置波特率
		UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;					//8位数据位
		UART_Handle.Init.StopBits = UART_STOPBITS_1;							//一位停止位
		UART_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;					//无硬件控流
		UART_Handle.Init.Mode = UART_MODE_TX_RX;									//收发模式
		UART_Handle.Init.Parity = UART_PARITY_NONE;								//无奇偶校验位
		UART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;			//采样速度：16
		
		HAL_UART_Init(&UART_Handle);													//初始化串口1
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)			//底层驱动
{
	if(huart->Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();					//开启GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();				//开启串口1的时钟
		
		GPIO_InitTypeDef GPIO_Hanndle;
		
		GPIO_Hanndle.Pin = GPIO_PIN_9;
		GPIO_Hanndle.Mode = GPIO_MODE_AF_PP;						//复用推挽输出
		GPIO_Hanndle.Speed = GPIO_SPEED_FREQ_HIGH ;
		HAL_GPIO_Init(GPIOA,&GPIO_Hanndle);
		
		GPIO_Hanndle.Pin = GPIO_PIN_10;
		GPIO_Hanndle.Mode = GPIO_MODE_INPUT;					//浮空输入
		GPIO_Hanndle.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA,&GPIO_Hanndle);
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		HAL_NVIC_SetPriority(USART1_IRQn,0,0);
	}
}


void USART1_IRQHandler(void)				//中断服务函数
{
	HAL_UART_IRQHandler(&UART_Handle);
	HAL_UART_Receive_IT(&UART_Handle,&aRxBuffer,1);					//接收中断
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)					//接收中断回调函数
{
	if(huart->Instance == USART1)
	{
		if(Uart1_cnt >= 255)				//溢出判断
		{
			Uart1_cnt = 0;
			memset(Uart1_RxBuff,0x00,sizeof(Uart1_RxBuff));
			HAL_UART_Transmit(&UART_Handle, (unsigned char *)&cAlmStr,sizeof(cAlmStr),0xff);
		}
		else
		{
			Uart1_RxBuff[Uart1_cnt++] = aRxBuffer;										//把数据存储到接收数据缓存里
			if(Uart1_RxBuff[Uart1_cnt-1] == '\n' && Uart1_RxBuff[Uart1_cnt-2] == '\r')				//判断帧尾(0x0a:\n 0x0d:\r)
			{
				HAL_UART_Transmit(&UART_Handle, (unsigned char *)&Uart1_RxBuff,sizeof(Uart1_RxBuff),0xff);				//将收到的信息发送出去
				printf("%s\r\n",Uart1_RxBuff);										//打印接收到的内容
				printf("len:%d\r\n",Uart1_cnt-2);									//打印接收到的内容的长度
				
				/*把本次接收的数据清零 计数也清零*/
				Uart1_cnt = 0;						//计数也清零
				memset(Uart1_RxBuff,0x00,sizeof(Uart1_RxBuff));					//数组清零	
			}
			HAL_UART_Receive_IT(&UART_Handle,&aRxBuffer,1);						//重新打开接收中断
		}
	}
}

int fputc(int c, FILE *stream)    //重写fputc函数
{ 
	HAL_UART_Transmit(&UART_Handle, (unsigned char *)&c, 1, 1000);
	return 1;
}
