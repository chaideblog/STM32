#include"stm32f10x.h"
#include"System_Configuration.h"
#include"LCD.h"
u8 Key_Pressed_Cnt=0;
u8 Key_F=1,Key_B=1;
u32 Delay_1ms_Cnt=0;
void Delay_1ms(u32 cnt);
void Key_Scan(void);
void SerialPutChar(u8 c);

int main(void)
{
  u8 i,j,k,m;
  RCC_Configuration(); //ʱ������
  NVIC_Configuration();//�ж�
  GPIO_Configuration();//IO
  TIM2_Configuration();//10ms ��ʱ��
  SysTick_delay_init(); //�趨SysTich Ƶ��Ϊ72M/72000=1ms
  //USART_Configuration(); //���ô���
  LCM_IO_Init();
  Init_ST7567();
  ClearScreen();    //clear all dots
  DisplayFont(1, 1, 17, FONT_8X16);
  DisplayFont(2, 8, 18, FONT_8X16);
  while(1)
  {
    clear_screen(0x00);
    int xuehao[11] = {1,3,0,1,0,1,3,0,0,7,6};
    for (j=0; j<11; j++)
    {
            m = xuehao[j];
            DisplayFont(1, j*8+1, m+17, FONT_8X16);
    }
    delay_ms(2000);
    for (j=0; j<16; j++)
    {
            k = j;
            DisplayFont(3, k*8+1, k+34, FONT_8X16);
    }
    delay_ms(2000);
    for (j=0; j<12; j++)
    {
            k = j;
            DisplayFont(5, k*8+1, k+48, FONT_8X16);
    }
    delay_ms(2000);
  
  }
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