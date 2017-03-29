#ifndef System_Configuration_H
#define System_Configuration_H
#include "stm32f10x.h"

extern void System_Configuration(void);
extern void RCC_Configuration(void);
extern void NVIC_Configuration(void);
extern void GPIO_Configuration(void);
extern void TIM2_Configuration(void);
extern void SysTick_Config(u32 con);
extern void SysTick_delay_init();	
extern void delay_us(u32 nus);
extern void delay_ms(u16 nms);
//******************************************
//       独立按键及LED初宏定义
//
//****************************************
#define ON   Bit_SET     //GPIO的布尔变量，用于GPIO_WriteBit函数
#define OFF  Bit_RESET   //GPIO的布尔变量，用于GPIO_WriteBit函数

#define LED0(x)  GPIO_WriteBit(GPIOA, GPIO_Pin_8, x)
#define LED1(x)  GPIO_WriteBit(GPIOC, GPIO_Pin_9, x)
#define LED2(x)  GPIO_WriteBit(GPIOC, GPIO_Pin_8, x)
#define LED3(x)  GPIO_WriteBit(GPIOC, GPIO_Pin_7, x)
#define LED4(x)  GPIO_WriteBit(GPIOC, GPIO_Pin_6, x)
#define LED5(x)  GPIO_WriteBit(GPIOG, GPIO_Pin_7, x)
#define LED6(x)  GPIO_WriteBit(GPIOG, GPIO_Pin_6, x)
#define LED7(x)  GPIO_WriteBit(GPIOB, GPIO_Pin_0, x)

#define KEY0  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)//读取按键3



#endif