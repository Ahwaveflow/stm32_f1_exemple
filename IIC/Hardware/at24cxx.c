#include "at24cxx.h"

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
    IIC_init();
}

#if EE_TYPE <= AT24C02 // 24C01/02/08
//��AT24CXXָ����ַ����һ������
// ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp = 0;
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Write_data(0XA0); //����д����
        IIC_Wait_Ack();
        IIC_Write_data(ReadAddr >> 8); //���͸ߵ�ַ
        IIC_Wait_Ack();
    }
    else
        IIC_Write_data(0XA0 + ((ReadAddr / 256) << 1)); //����������ַ0XA0,д����

    IIC_Wait_Ack();
    IIC_Write_data(ReadAddr % 256); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Write_data(0XA1); //�������ģʽ
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop(); //����һ��ֹͣ����
    return temp;
}
//��AT24CXXָ����ַд��һ������
// WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
// DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Write_data(0XA0); //�����豸��ַ
        IIC_Wait_Ack();
        IIC_Write_data(WriteAddr >> 8); //�����ֽڸߵ�ַ
    }
    else
    {
        IIC_Write_data(0XA0 + ((WriteAddr / 256) << 1)); //����������ַ0XA0,д����
    }
    IIC_Wait_Ack();
    IIC_Write_data(WriteAddr % 256); //�����ֽڵ͵�ַ
    IIC_Wait_Ack();
    IIC_Write_data(DataToWrite); //����Ҫд�������
    IIC_Wait_Ack();
    IIC_Stop(); //����һ��ֹͣ����
    delay_ms(10);
}
#else // 24C16
/*****************************************************************
*������: AT24CXX_ReadOneByte(u16 ReadAddr)
*����:AT24CXX ��ָ����ַ��һ���ֽ�   AT24C16ʹ��
*����:�ײ�I2C��д����
*������:�ⲿ����
*�β�:
            ReadAddr��Ҫ��ȡ�ĵ�ַ
*����ֵ:���ض�ȡ������
*����:ÿ�ζ�������һ��I2Cʱ��
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
    IIC_Write_data(DeviceAddress & 0xFE); //�����豸��ַ+д����
    IIC_Wait_Ack();
    IIC_Write_data(WordAddress); //�����ֽڵ�ַ
    IIC_Wait_Ack();
    IIC_Start();                          //��ʼ�ź�
    IIC_Write_data(DeviceAddress | 0x01); //�����豸��ַ+������
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop(); //����һ��ֹͣ����
    return temp;
}
/*****************************************************************
*������: AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
*����:AT24CXX ��ָ����ַд��һ���ֽ�   AT24C16ʹ��
*����:
*������:�ⲿ����
*�β�:
            WriteAddr��Ҫд��ĵ�ַ
            DataToWrite��д�������
*����ֵ:��
*����:ÿ��д������һ��I2Cʱ��
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
    IIC_Write_data(DeviceAddress); //�����豸��ַ
    IIC_Wait_Ack();
    IIC_Write_data(WordAddress); //�����ֽڵ�ַ
    IIC_Wait_Ack();
    IIC_Write_data(DataToWrite); //����Ҫд�������
    IIC_Wait_Ack();
    IIC_Stop(); //����һ��ֹͣ����
    delay_ms(10);
}
#endif

/*---------------��д��ʽѡ��-----------------*/
#if QuickWR == 0
//��AT24CXX�����ָ����ַ��ʼ����ָ������������
// ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
// pBuffer  :���������׵�ַ
// NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
// WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
// pBuffer   :���������׵�ַ
// NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    while (NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
#else //���ٶ�д��ʽ
/*****************************************************************
*������: AT24CXX_Write_Bytes(u8 *pBuffer,u16 WriteAddress,u8 Len)
*����: ҳд���� ���д��һҳ(16�ֽ�)
*����: �ײ�I2Cд����
*������:�ⲿ����
*�β�:
      *pBuffer��ָ��д�뻺����
            WriteAddr��Ҫд��ĵ�ַ
            Len��д�����ݳ���
*����ֵ:��
*����:����һ��I2Cʱ�����д��һҳ(16Bytes)����,���Կ��ڰ��ֽ�д��
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
    IIC_Write_data(DeviceAddress); //�����豸��ַ
    IIC_Wait_Ack();
    IIC_Write_data(WordAddress); //�����ֽڵ�ַ
    IIC_Wait_Ack();
    for (i = 0; i < Len; i++)
    {
        IIC_Write_data(*pBuffer++); //�����ֽڵ�ַ
        IIC_Wait_Ack();
    }
    IIC_Stop(); //����һ��ֹͣ����
    delay_ms(10);
}
/*****************************************************************
*������: AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
*����:AT24CXX ����д�벻�����ֽ�
*����:
*������:�ⲿ����
*�β�:
            WriteAddr��Ҫд����׵�ַ
            *pBuffer��ָ��д�뻺����
            NumToWrite��д����ֽ���
*����ֵ:��
*����:����ģʽ ����ÿ��дһ���ֽھ�����һ��I2Cʱ��
*****************************************************************/
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr = WriteAddr % AT24CXX_Page_Size;         //��ַ������16�ֽڶ���
    count = AT24CXX_Page_Size - Addr;             //�������ֽ���
    NumOfPage = NumToWrite / AT24CXX_Page_Size;   //��Ҫд�����ҳ
    NumOfSingle = NumToWrite % AT24CXX_Page_Size; //ʣ����д����ֽ���
    if (0 == Addr)                                //�����ַ����
    {
        if (NumToWrite <= AT24CXX_Page_Size) //д���ֽ�<=1ҳ
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumToWrite);
        }
        else
        {
            while (NumOfPage--) //��ҳд��
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, AT24CXX_Page_Size);
                pBuffer += AT24CXX_Page_Size;
                WriteAddr += AT24CXX_Page_Size;
            }
            if (NumOfSingle != 0) //�����ʣ���ֽ�
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumOfSingle); //��ʣ�µ��ֽ�д��
            }
        }
    }
    else //��ַ������
    {
        if (NumToWrite <= count) // Ҫд����ֽ���<=count
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumToWrite); //д��ʵ���ֽ���
        }
        else //Ҫд���ֽ�������count
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, count); //�ֽ�count���ֽ�д�� д��� ��ַ�պö���
            NumToWrite -= count;                            //����ʣ���ֽ���
            pBuffer += count;                               //д������ƫ��count
            WriteAddr += count;                             //д���ַƫ��count

            NumOfPage = NumToWrite / AT24CXX_Page_Size;   //��Ҫд�����ҳ
            NumOfSingle = NumToWrite % AT24CXX_Page_Size; //ʣ����д����ֽ���

            while (NumOfPage--) //�Ȱ�ҳд��
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, AT24CXX_Page_Size);
                pBuffer += AT24CXX_Page_Size;
                WriteAddr += AT24CXX_Page_Size;
            }
            if (NumOfSingle != 0) //��ʣ���ֽ�
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumOfSingle); //��ʣ�µ��ֽ�д��
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

#endif //���ٶ�д��ʽ

//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
    u8 temp;
    temp = AT24CXX_ReadOneByte(255); //����ÿ�ο�����дAT24CXX
    if (temp == 0X55)
        return 0;
    else //�ų���һ�γ�ʼ�������
    {
        AT24CXX_WriteOneByte(255, 0X55);
        temp = AT24CXX_ReadOneByte(255);
        if (temp == 0X55)
            return 0;
    }
    return 1;
}
