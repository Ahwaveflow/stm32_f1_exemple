#include "uart.h"

uint8_t aRxBuffer;								//�����жϻ���
uint8_t Uart1_RxBuff[256];				//���ջ���
uint8_t Uart1_cnt = 0;						//���ջ������
uint8_t cAlmStr[] = "�������(����256)\r\n";

UART_HandleTypeDef UART_Handle;				//���ھ��

void Uart_Init(uint32_t Bound)
{
		UART_Handle.Instance = USART1;												//����1
		UART_Handle.Init.BaudRate = Bound;										//���ò�����
		UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;					//8λ����λ
		UART_Handle.Init.StopBits = UART_STOPBITS_1;							//һλֹͣλ
		UART_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;					//��Ӳ������
		UART_Handle.Init.Mode = UART_MODE_TX_RX;									//�շ�ģʽ
		UART_Handle.Init.Parity = UART_PARITY_NONE;								//����żУ��λ
		UART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;			//�����ٶȣ�16
		
		HAL_UART_Init(&UART_Handle);													//��ʼ������1
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)			//�ײ�����
{
	if(huart->Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();					//����GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();				//��������1��ʱ��
		
		GPIO_InitTypeDef GPIO_Hanndle;
		
		GPIO_Hanndle.Pin = GPIO_PIN_9;
		GPIO_Hanndle.Mode = GPIO_MODE_AF_PP;						//�����������
		GPIO_Hanndle.Speed = GPIO_SPEED_FREQ_HIGH ;
		HAL_GPIO_Init(GPIOA,&GPIO_Hanndle);
		
		GPIO_Hanndle.Pin = GPIO_PIN_10;
		GPIO_Hanndle.Mode = GPIO_MODE_INPUT;					//��������
		GPIO_Hanndle.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA,&GPIO_Hanndle);
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		HAL_NVIC_SetPriority(USART1_IRQn,0,0);
	}
}


void USART1_IRQHandler(void)				//�жϷ�����
{
	HAL_UART_IRQHandler(&UART_Handle);
	HAL_UART_Receive_IT(&UART_Handle,&aRxBuffer,1);					//�����ж�
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)					//�����жϻص�����
{
	if(huart->Instance == USART1)
	{
		if(Uart1_cnt >= 255)				//����ж�
		{
			Uart1_cnt = 0;
			memset(Uart1_RxBuff,0x00,sizeof(Uart1_RxBuff));
			HAL_UART_Transmit(&UART_Handle, (unsigned char *)&cAlmStr,sizeof(cAlmStr),0xff);
		}
		else
		{
			Uart1_RxBuff[Uart1_cnt++] = aRxBuffer;										//�����ݴ洢���������ݻ�����
			if(Uart1_RxBuff[Uart1_cnt-1] == '\n' && Uart1_RxBuff[Uart1_cnt-2] == '\r')				//�ж�֡β(0x0a:\n 0x0d:\r)
			{
				HAL_UART_Transmit(&UART_Handle, (unsigned char *)&Uart1_RxBuff,sizeof(Uart1_RxBuff),0xff);				//���յ�����Ϣ���ͳ�ȥ
				printf("%s\r\n",Uart1_RxBuff);										//��ӡ���յ�������
				printf("len:%d\r\n",Uart1_cnt-2);									//��ӡ���յ������ݵĳ���
				
				/*�ѱ��ν��յ��������� ����Ҳ����*/
				Uart1_cnt = 0;						//����Ҳ����
				memset(Uart1_RxBuff,0x00,sizeof(Uart1_RxBuff));					//��������	
			}
			HAL_UART_Receive_IT(&UART_Handle,&aRxBuffer,1);						//���´򿪽����ж�
		}
	}
}

int fputc(int c, FILE *stream)    //��дfputc����
{ 
	HAL_UART_Transmit(&UART_Handle, (unsigned char *)&c, 1, 1000);
	return 1;
}
