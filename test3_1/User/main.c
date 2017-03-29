#include"stm32f10x.h"
#include"System_Configuration.h"

u8 Key_Pressed_Cnt=0;
u8 Key_F=1,Key_B=1;
u32 Delay_1ms_Cnt=0;
u8 getChar;
void Delay_1ms(u32 cnt);
void Key_Scan(void);
void SerialPutChar(u8 c);
void Serial_PutString(u8 *s);


int main(void)
{
  RCC_Configuration(); //时钟配置
  NVIC_Configuration();//中断
  GPIO_Configuration();//IO
  TIM2_Configuration();//10ms 定时器
  SysTick_delay_init(); //设定SysTich 频率为72M/72000=1ms
  USART_Configuration(); //配置串口
  
  SerialPutChar('\0');
  Serial_PutString("Hello World! \n");
  Serial_PutString("Chai Jinlong 13010130076 ! \n");
  
  while(1){
    Delay_1ms(100);
  }
  //SerialPutChar(getChar);
  //u8 num = getChar;
}



void Key_Scan(void)
{
 
  Key_F=KEY2;
  if((Key_F==0)&&(Key_B==1))
  {
    Key_Pressed_Cnt++;//检测到按键
    if(Key_Pressed_Cnt==2)
      Key_Pressed_Cnt=0;
  }
  Key_B=Key_F;
}

void Delay_1ms(u32 cnt)
{
  Delay_1ms_Cnt=cnt;
  while(Delay_1ms_Cnt); //等待进入SysTick中断cnt此后，退出Delay_1ms函数
}
/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void SerialPutChar(u8 c)
{
  USART_SendData(USART1, c);                                  //简要发送的数据填充到发送缓冲区
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//等待发送结束
}
/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial_PutString(u8 *s)
{
  while (*s != '\0')
  {
    SerialPutChar(*s);
    s ++;
  }
}
/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial_PutBuffer(u8 *s, u8 Length)
{
  while (Length--)
  {
    SerialPutChar(*s);
    s ++;
  }
}