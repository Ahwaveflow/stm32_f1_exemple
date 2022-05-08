#include "iic.h"

void IIC_init(void)
{
    GPIO_InitTypeDef iic_Handle;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    iic_Handle.Pin = GPIO_PIN_8 | GPIO_PIN_9;       
    iic_Handle.Mode = GPIO_MODE_OUTPUT_PP;          //推挽输出
    iic_Handle.Speed = GPIO_SPEED_FREQ_HIGH;
    iic_Handle.Pull = GPIO_PULLUP;      //上拉

    HAL_GPIO_Init(GPIOB,&iic_Handle);       //GPIO初始化

    IIC_SCL = 1;        //拉高时钟线程
    IIC_SDA = 1;        //拉高数据线程
}

/*发送IIC开始信号  ( SCL = 1时，SDA 1 --> 0 )*/
void IIC_Start(void)
{
    SDA_OUT();          //输出模式
    IIC_SCL = 1;
    IIC_SDA = 1;
    delay_us(5);        //延时5us
    IIC_SDA = 0;        //拉低数据线
    delay_us(5);
    IIC_SCL = 0;        //拉低SCL准备传输数据或接收数据
    delay_us(5);
}

/*发送IIC停止信号  ( SCL = 1时，SDA 0 --> 1 )*/
void IIC_Stop(void)
{
    SDA_OUT();      //输出模式
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(5);
    IIC_SCL = 1;
    delay_us(5);
    IIC_SDA = 1;    //拉高数据线
    delay_us(5);
}

/*等待从机应答信号到来*/
//返回值：NACK (1) ,接收应答失败
//        ACK (0) ,接收应答成功 
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();           //SDA设置为输入模式
    IIC_SCL = 0;
    delay_us(5);
    IIC_SCL = 1;
    delay_us(2);
    while(INPUT_SDA) 
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return NACK;
        }
    }
    delay_us(2);
    IIC_SCL = 0;
    return ACK;
}

/*主机产生应答*/
void IIC_Ack(u8 ack)
{
    IIC_SCL = 0;
    SDA_OUT();
    if(ack)
    {
        IIC_SDA = 1;        //不应答
    }
    else
    {
        IIC_SDA = 0;        //应答
    }
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    IIC_SCL = 0;
}

// /*主机不产生应答*/
// void IIC_Nack(void)
// {
//     IIC_SCL = 0;
//     SDA_OUT();
//     IIC_SDA = 1;
//     delay_us(2);
//     IIC_SCL = 1;
//     delay_us(2);
//     IIC_SCL = 0;   
// }

/*
    主机写入数据到外设中
    参数值： data  要写入的数据
*/
void IIC_Write_data(u8 data)
{
    u8 i;
    SDA_OUT();
    IIC_SCL = 0;
    delay_us(5);
    for(i = 0;i < 8;i++)
    {
        IIC_SDA = (data & 0x80)>>7;     //从高位依次写入
        delay_us(2);
        IIC_SCL = 1;
        delay_us(5);
        IIC_SCL = 0;
        delay_us(2);
        data <<= 1;                 //写入下一位字节
    }
}

/*
    主机从从外设中读取数据
    参考值： Ack（有无应答）
    返回值： receive
*/
u8 IIC_Read_Byte(u8 Ack)
{
    u8 i,receive = 0;

    SDA_IN();
    for(i = 0;i < 8;i++)
    {
        IIC_SCL = 0;
        delay_us(2);
        IIC_SCL = 1;
        if(INPUT_SDA)
            receive++;
        delay_us(4);
        receive <<= 1;
    }
    if(!Ack)
        IIC_Ack(Ack);   //应答
    else
        IIC_Ack(Ack);   //不应答

    return receive;
}

