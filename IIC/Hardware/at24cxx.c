#include "at24cxx.h"

//初始化IIC接口
void AT24CXX_Init(void)
{
    IIC_init();
}

#if EE_TYPE <= AT24C02 // 24C01/02/08
//在AT24CXX指定地址读出一个数据
// ReadAddr:开始读数的地址
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp = 0;
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Write_data(0XA0); //发送写命令
        IIC_Wait_Ack();
        IIC_Write_data(ReadAddr >> 8); //发送高地址
        IIC_Wait_Ack();
    }
    else
        IIC_Write_data(0XA0 + ((ReadAddr / 256) << 1)); //发送器件地址0XA0,写数据

    IIC_Wait_Ack();
    IIC_Write_data(ReadAddr % 256); //发送低地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Write_data(0XA1); //进入接收模式
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop(); //产生一个停止条件
    return temp;
}
//在AT24CXX指定地址写入一个数据
// WriteAddr  :写入数据的目的地址
// DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Write_data(0XA0); //发送设备地址
        IIC_Wait_Ack();
        IIC_Write_data(WriteAddr >> 8); //发送字节高地址
    }
    else
    {
        IIC_Write_data(0XA0 + ((WriteAddr / 256) << 1)); //发送器件地址0XA0,写数据
    }
    IIC_Wait_Ack();
    IIC_Write_data(WriteAddr % 256); //发送字节低地址
    IIC_Wait_Ack();
    IIC_Write_data(DataToWrite); //发送要写入的数据
    IIC_Wait_Ack();
    IIC_Stop(); //产生一个停止条件
    delay_ms(10);
}
#else // 24C16
/*****************************************************************
*函数名: AT24CXX_ReadOneByte(u16 ReadAddr)
*功能:AT24CXX 读指定地址的一个字节   AT24C16使用
*调用:底层I2C读写函数
*被调用:外部调用
*形参:
            ReadAddr：要读取的地址
*返回值:返回读取的数据
*其他:每次读就启动一次I2C时序
*****************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0xA0;
    u8 temp = 0;
    Page = ReadAddr / AT24CXX_Page_Size;
    WordAddress = (ReadAddr % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
    IIC_Start();
    IIC_Write_data(DeviceAddress & 0xFE); //发送设备地址+写方向
    IIC_Wait_Ack();
    IIC_Write_data(WordAddress); //发送字节地址
    IIC_Wait_Ack();
    IIC_Start();                          //起始信号
    IIC_Write_data(DeviceAddress | 0x01); //发送设备地址+读方向
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop(); //产生一个停止条件
    return temp;
}
/*****************************************************************
*函数名: AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
*功能:AT24CXX 向指定地址写入一个字节   AT24C16使用
*调用:
*被调用:外部调用
*形参:
            WriteAddr：要写入的地址
            DataToWrite：写入的数据
*返回值:无
*其他:每次写就启动一次I2C时序
*****************************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0xA0;
    Page = WriteAddr / AT24CXX_Page_Size;
    WordAddress = (WriteAddr % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
#if DEBUG > 0
    printf("Page:%x\r\n", Page);
    printf("WordAddress:%x\r\n", WordAddress);
    printf("DeviveAddress:%x\r\n", DeviceAddress);
#endif
    IIC_Start();
    IIC_Write_data(DeviceAddress); //发送设备地址
    IIC_Wait_Ack();
    IIC_Write_data(WordAddress); //发送字节地址
    IIC_Wait_Ack();
    IIC_Write_data(DataToWrite); //发送要写入的数据
    IIC_Wait_Ack();
    IIC_Stop(); //产生一个停止条件
    delay_ms(10);
}
#endif

/*---------------读写方式选择-----------------*/
#if QuickWR == 0
//在AT24CXX里面的指定地址开始读出指定个数的数据
// ReadAddr :开始读出的地址 对24c02为0~255
// pBuffer  :数据数组首地址
// NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
// WriteAddr :开始写入的地址 对24c02为0~255
// pBuffer   :数据数组首地址
// NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    while (NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
#else //快速读写方式
/*****************************************************************
*函数名: AT24CXX_Write_Bytes(u8 *pBuffer,u16 WriteAddress,u8 Len)
*功能: 页写函数 最多写入一页(16字节)
*调用: 底层I2C写函数
*被调用:外部调用
*形参:
      *pBuffer：指向写入缓存区
            WriteAddr：要写入的地址
            Len：写入数据长度
*返回值:无
*其他:启动一次I2C时序最多写入一页(16Bytes)数据,明显快于按字节写入
*****************************************************************/
void AT24CXX_Write_Bytes(u8 *pBuffer, u16 WriteAddress, u8 Len)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0xA0;
    u8 i = 0;
    Page = WriteAddress / AT24CXX_Page_Size;
    WordAddress = (WriteAddress % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
    IIC_Start();
    IIC_Write_data(DeviceAddress); //发送设备地址
    IIC_Wait_Ack();
    IIC_Write_data(WordAddress); //发送字节地址
    IIC_Wait_Ack();
    for (i = 0; i < Len; i++)
    {
        IIC_Write_data(*pBuffer++); //发送字节地址
        IIC_Wait_Ack();
    }
    IIC_Stop(); //产生一个停止条件
    delay_ms(10);
}
/*****************************************************************
*函数名: AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
*功能:AT24CXX 快速写入不定量字节
*调用:
*被调用:外部调用
*形参:
            WriteAddr：要写入的首地址
            *pBuffer：指向写入缓存区
            NumToWrite：写入的字节数
*返回值:无
*其他:快速模式 不用每次写一个字节就启动一次I2C时序
*****************************************************************/
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr = WriteAddr % AT24CXX_Page_Size;         //地址正好是16字节对齐
    count = AT24CXX_Page_Size - Addr;             //不对齐字节数
    NumOfPage = NumToWrite / AT24CXX_Page_Size;   //需要写入多少页
    NumOfSingle = NumToWrite % AT24CXX_Page_Size; //剩余需写入的字节数
    if (0 == Addr)                                //如果地址对齐
    {
        if (NumToWrite <= AT24CXX_Page_Size) //写入字节<=1页
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumToWrite);
        }
        else
        {
            while (NumOfPage--) //按页写入
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, AT24CXX_Page_Size);
                pBuffer += AT24CXX_Page_Size;
                WriteAddr += AT24CXX_Page_Size;
            }
            if (NumOfSingle != 0) //如果还剩下字节
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumOfSingle); //把剩下的字节写入
            }
        }
    }
    else //地址不对齐
    {
        if (NumToWrite <= count) // 要写入的字节数<=count
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumToWrite); //写入实际字节数
        }
        else //要写入字节数大于count
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, count); //现将count个字节写入 写入后 地址刚好对齐
            NumToWrite -= count;                            //计算剩余字节数
            pBuffer += count;                               //写入内容偏移count
            WriteAddr += count;                             //写入地址偏移count

            NumOfPage = NumToWrite / AT24CXX_Page_Size;   //需要写入多少页
            NumOfSingle = NumToWrite % AT24CXX_Page_Size; //剩余需写入的字节数

            while (NumOfPage--) //先按页写入
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, AT24CXX_Page_Size);
                pBuffer += AT24CXX_Page_Size;
                WriteAddr += AT24CXX_Page_Size;
            }
            if (NumOfSingle != 0) //还剩余字节
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumOfSingle); //把剩下的字节写入
            }
        }
    }
}

void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0x50;
    Page = ReadAddr / AT24CXX_Page_Size;
    WordAddress = (ReadAddr % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
    while (NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

#endif //快速读写方式

//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
    u8 temp;
    temp = AT24CXX_ReadOneByte(255); //避免每次开机都写AT24CXX
    if (temp == 0X55)
        return 0;
    else //排除第一次初始化的情况
    {
        AT24CXX_WriteOneByte(255, 0X55);
        temp = AT24CXX_ReadOneByte(255);
        if (temp == 0X55)
            return 0;
    }
    return 1;
}
