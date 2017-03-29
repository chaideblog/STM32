#include"stm32f10x.h"

void GPIO_Configuration(void);

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

void Delay1ms(unsigned int cnt_1ms)
{
  unsigned int i;
  while(cnt_1ms--)   //默认系统主时钟72M，反汇编效率约1/8,故一个周期约为1ms
  {
    i=9000;             
    while(i--);
  }
}

int main(void)
{
  GPIO_Configuration();//IO初始化

  while(1)
  {  
    if(KEY0 == 0)
    {
       LED0(ON);Delay1ms(100);
       LED1(ON);Delay1ms(100);
       LED2(ON);Delay1ms(100);
       LED3(ON);Delay1ms(100);
       LED4(ON);Delay1ms(100);
       LED5(ON);Delay1ms(100);
       LED6(ON);Delay1ms(100);
       LED7(ON);Delay1ms(100);
    }
  }
}

/***************************************************************************
*名    称：GPIO_Configuration
*功    能：配置相应的IO引脚
*入口参数：无
*出口参数：无
*说    明：需要开启相应时钟源，GPIO_Pin,GPIO_Mode,GPIO_Speed
*范    例：
****************************************************************************/
void GPIO_Configuration(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
  //按键初始化函数  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG, ENABLE);//使能PA,PB,PC,PG,PF端口时钟
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//KEY0-KEY2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PC0设置成输入， KEY3
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
 //LED IO初始化
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);				//根据设定参数初始化
 GPIO_ResetBits(GPIOA,GPIO_Pin_8);		                //LED0
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);				 //LED7
 GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//LED1,LED2,LED3,LED4,LED5

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOG, &GPIO_InitStructure);				 //根据设定参数初始
 GPIO_ResetBits(GPIOG, GPIO_Pin_6|GPIO_Pin_7);                  //LED6,LED7		 

}

 