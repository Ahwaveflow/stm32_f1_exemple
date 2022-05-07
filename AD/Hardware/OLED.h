
#ifndef __OLED_H__
#define __OLED_H__
			  	 
//========移植的时候，根据所选芯片引入相应的头文件==========
#include "stm32f1xx_hal.h"
	
	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	  
  						  
//========移植的时候，根据引脚分配进行修改==========

/*

SCLK ->PB15
SDIN -> PB14
RES -> PB13
DC ->PB12
CS ->GND
 
*/


//使用4线串行接口时使用 
//#define OLED_CS_GPIO			GPIOB
//#define OLED_CS_GPIO_PIN			GPIO_PIN_12
#define OLED_DC_GPIO			GPIOB
#define OLED_DC_GPIO_PIN			GPIO_PIN_12
#define OLED_SCLK_GPIO			GPIOB
#define OLED_SCLK_GPIO_PIN		GPIO_PIN_15
#define OLED_SDIN_GPIO			GPIOB
#define OLED_SDIN_GPIO_PIN		GPIO_PIN_14
#define OLED_RES_GPIO 			GPIOB
#define OLED_RES_GPIO_PIN 			GPIO_PIN_13



//CS//没有使用片选
//#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO, OLED_CS_GPIO_PIN, GPIO_PIN_RESET)
//#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO, OLED_CS_GPIO_PIN, GPIO_PIN_SET)

//RES
#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO, OLED_RES_GPIO_PIN, GPIO_PIN_RESET)//NOT USE  0
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RES_GPIO, OLED_RES_GPIO_PIN, GPIO_PIN_SET)//NOT USE   1

//DC
#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO, OLED_DC_GPIO_PIN, GPIO_PIN_RESET)
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO, OLED_DC_GPIO_PIN, GPIO_PIN_SET)

//SCLK,D0
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCLK_GPIO, OLED_SCLK_GPIO_PIN, GPIO_PIN_RESET)
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCLK_GPIO, OLED_SCLK_GPIO_PIN, GPIO_PIN_SET)

//SDIN,D1
#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDIN_GPIO, OLED_SDIN_GPIO_PIN, GPIO_PIN_RESET)
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDIN_GPIO, OLED_SDIN_GPIO_PIN, GPIO_PIN_SET)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);	    
void OLED_Display_On(void);//打开OLED
void OLED_Display_Off(void);//关闭OLED				   		    
void OLED_Init(void);//初始化OLED
void OLED_Clear(void);//清屏
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr);//显示一个字符
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size);//显示数字
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p);//显示字符串 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);//显示中文字
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);//显示图片
void Display_bmp(void);
#endif  
	 



