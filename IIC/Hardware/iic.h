#ifndef __IIC_H
#define __IIC_H

#include "stm32f1xx_hal.h"
#include "main.h"

#define ACK  0      //应答
#define NACK 1      //不应答

#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}           //上拉输入
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}           //推挽输出

#define IIC_SCL   PBout(8)        //SCL
#define IIC_SDA   PBout(9)        //输出SDA
#define INPUT_SDA PBin(9)         //输入SDA


void IIC_init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(u8 ack);
// void IIC_Nack(void);
void IIC_Write_data(u8 data);
u8 IIC_Wait_Ack(void);
u8 IIC_Read_Byte(u8 Ack);



#endif

