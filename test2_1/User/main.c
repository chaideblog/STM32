#include"stm32f10x.h"
#include"System_Configuration.h"


void Delay1ms(unsigned int cnt_1ms)
{
  unsigned int i;
  while(cnt_1ms--)   //默认系统主时钟72M，反汇编效率约1/8,故一个周期约为1ms
  {
    i=8000;             
    while(i--);
  }
}

u8 Key_Pressed_Flag=0;

int main(void)
{
  unsigned int STATE = 1;
  RCC_Configuration(); //时钟配置
  NVIC_Configuration();//中断
  GPIO_Configuration();//IO
  
  while(1)
  { 
    if(Key_Pressed_Flag == 1)
    {
      STATE = (STATE + 1) % 2;
      LED1(STATE);Delay1ms(1000);
      Key_Pressed_Flag=0;
    }
  }
}

 