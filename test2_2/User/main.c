#include"stm32f10x.h"
#include"System_Configuration.h"

u8 Key_Pressed_Cnt=0;
u8 Key_F=1,Key_B=1;
u32 Delay_1ms_Cnt=0;
void Delay_1ms(u32 cnt)
{
  Delay_1ms_Cnt=cnt;
  while(Delay_1ms_Cnt); //等待进入SysTick中断cnt此后，退出Delay_1ms函数
}
int main(void)
{
  RCC_Configuration(); //时钟配置
  NVIC_Configuration();//中断
  GPIO_Configuration();//IO
  TIM2_Configuration();//10ms 定时器
  SysTick_delay_init(); //设定SysTich 频率为72M/72000=1ms
  
  
  while(1)
  {  
    switch(Key_Pressed_Cnt)
    {
    case 0:
      LED0(ON);delay_ms(100);LED0(OFF);delay_ms(100);
      LED1(ON);delay_ms(100);LED1(OFF);delay_ms(100);
      LED2(ON);delay_ms(100);LED2(OFF);delay_ms(100);
      LED3(ON);delay_ms(100);LED3(OFF);delay_ms(100);
      LED4(ON);delay_ms(100);LED4(OFF);delay_ms(100);
      LED5(ON);delay_ms(100);LED5(OFF);delay_ms(100);
      LED6(ON);delay_ms(100);LED6(OFF);delay_ms(100);
      LED7(ON);delay_ms(100);LED7(OFF);delay_ms(100);
      break;
    case 1:
      LED7(ON);delay_ms(100);LED7(OFF);delay_ms(100);
      LED6(ON);delay_ms(100);LED6(OFF);delay_ms(100);
      LED5(ON);delay_ms(100);LED5(OFF);delay_ms(100);
      LED4(ON);delay_ms(100);LED4(OFF);delay_ms(100);
      LED3(ON);delay_ms(100);LED3(OFF);delay_ms(100);
      LED2(ON);delay_ms(100);LED2(OFF);delay_ms(100);
      LED1(ON);delay_ms(100);LED1(OFF);delay_ms(100);
      LED0(ON);delay_ms(100);LED0(OFF);delay_ms(100);
      break;
    default:break;
    }
  }
}

void Key_Scan(void)
{
  Key_F=KEY0;
  if((Key_F==0)&&(Key_B==1))
  {
    Key_Pressed_Cnt=0;
  }
  Key_B=Key_F;
  
  Key_F=KEY1;
  if((Key_F==0)&&(Key_B==1))
  {
    Key_Pressed_Cnt=1;
  }
  Key_B=Key_F;
 /*
  Key_F=KEY2;
  if((Key_F==0)&&(Key_B==1))
  {
    Key_Pressed_Cnt++;//检测到按键
    if(Key_Pressed_Cnt==2)
      Key_Pressed_Cnt=0;
  }
  Key_B=Key_F;
  */
}
