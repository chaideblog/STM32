#include"stm32f10x.h"
#include"System_Configuration.h"


void Delay1ms(unsigned int cnt_1ms)
{
  unsigned int i;
  while(cnt_1ms--)   //Ĭ��ϵͳ��ʱ��72M�������Ч��Լ1/8,��һ������ԼΪ1ms
  {
    i=8000;             
    while(i--);
  }
}

u8 Key_Pressed_Flag=0;

int main(void)
{
  unsigned int STATE = 1;
  RCC_Configuration(); //ʱ������
  NVIC_Configuration();//�ж�
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

 