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
  RCC_Configuration(); //ʱ������
  NVIC_Configuration();//�ж�
  GPIO_Configuration();//IO
  TIM2_Configuration();//10ms ��ʱ��
  SysTick_delay_init(); //�趨SysTich Ƶ��Ϊ72M/72000=1ms
  USART_Configuration(); //���ô���
  
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
    Key_Pressed_Cnt++;//��⵽����
    if(Key_Pressed_Cnt==2)
      Key_Pressed_Cnt=0;
  }
  Key_B=Key_F;
}

void Delay_1ms(u32 cnt)
{
  Delay_1ms_Cnt=cnt;
  while(Delay_1ms_Cnt); //�ȴ�����SysTick�ж�cnt�˺��˳�Delay_1ms����
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
  USART_SendData(USART1, c);                                  //��Ҫ���͵�������䵽���ͻ�����
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ����ͽ���
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