#ifndef _LCM_H
#define _LCM_H

#include "stm32f10x.h"


typedef enum
{
	FONT_5X8 = 1,
	FONT_8X16,
	FONT_16X16,
	FONT_128X64,
	SCRIBING
}Typeface;

/*
LCM_RESET  PB9 
LCM_CSB    PB5  
LCM_A0     PG8  RS 
I2C1_SDA_LCM    PB7
L2C1_SCL_LCM		PB6
*/

#define LCM_RS(x)	GPIO_WriteBit(GPIOG, GPIO_Pin_8, x)
#define LCM_RESET(x)    GPIO_WriteBit(GPIOB, GPIO_Pin_9, x)
#define LCM_CSB(x)	GPIO_WriteBit(GPIOB, GPIO_Pin_5, x)
#define LCM_SDA(x)      GPIO_WriteBit(GPIOB, GPIO_Pin_7, x)
#define LCM_SCL(x)      GPIO_WriteBit(GPIOB, GPIO_Pin_6, x)

void LCM_IO_Init(void);
void Init_ST7567(void);
void clear_screen(uint8_t data);
void Display_pic1(void);
void DisplayFont(uint8_t page, uint8_t column, uint8_t No, uint8_t typeface);
void ClearScreen(void);
void Display_PS(void);

extern const uint8_t graphicPS[];
extern void Display_128x64(u8 *dp);
#endif
