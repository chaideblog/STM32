#include "stm32f10x.h"
#include "seg.h"

extern void Delay1ms(unsigned int cnt_1ms);
uint8_t data[10]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};   //0-9���������ʾ���

/*
SEG_led0 --PE2
SEG led1 --PE3
SEG led2 --PB1
SEG led3 --PB8
SEG led4 --PG14
SEG led5 --PG13
SEG led6 --PD2
SEG led_DP --PG11 

SEG led_s0 --PD7
SEG led_s1 --PG9
SEG led_s2 --PD6
SEG led_s3 --PD3
*/

/************************************�ܽ�����************************************/
void SEG_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //�ܽ�����Ϊ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //�������50MHz
  GPIO_Init(GPIOB,&GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //�ܽ�����Ϊ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //�������50MHz
  GPIO_Init(GPIOE,&GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11  | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //�ܽ�����Ϊ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //�������50MHz
  GPIO_Init(GPIOG,&GPIO_InitStructure);

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //�ܽ�����Ϊ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //�������50MHz
  GPIO_Init(GPIOG,&GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_2 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD,&GPIO_InitStructure);

}

void show(u8 pl) //��p1����ʾ��ת���ɶ���������ʾ����8����ʾ��a��b��c��d��e��f��g��Dp
{
 uint8_t data[10]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};   //0-9���������ʾ���
  BitAction sum[8];
  u8 i;
  for(i=0;i<8;i++)
  {
    if((data[pl]>>i)&0x01 == 0x1)	//�������Ҫ��ʾ�����ݣ�תΪÿ��LED�ĹܽŵĲ�������
            sum[i] = Bit_SET;
    else	
            sum[i] = Bit_RESET;
  }
  GPIO_WriteBit(GPIOE,GPIO_Pin_2,sum[7]);
  GPIO_WriteBit(GPIOE,GPIO_Pin_3,sum[6]);
  GPIO_WriteBit(GPIOB,GPIO_Pin_1,sum[5]);
  GPIO_WriteBit(GPIOB,GPIO_Pin_8,sum[4]);
  GPIO_WriteBit(GPIOG,GPIO_Pin_14,sum[3]);
  GPIO_WriteBit(GPIOG,GPIO_Pin_13,sum[2]);
  GPIO_WriteBit(GPIOD,GPIO_Pin_2,sum[1]);
  GPIO_WriteBit(GPIOG,GPIO_Pin_11,sum[0]);
}

void ShowNunber(uint8_t Nunber)
{
    uint8_t temp1,temp2;
    if(Nunber < 16 && Nunber > 9)
    {
        temp1 = Nunber /10;   //ȡ��ʮλ��
        temp2 = Nunber - temp1*10;  //ȡ����λ��
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);
        GPIO_ResetBits(GPIOG,GPIO_Pin_9);
        GPIO_SetBits(GPIOD,GPIO_Pin_6);
        GPIO_ResetBits(GPIOD,GPIO_Pin_3);
        show(temp1);
        Delay1ms(10);
       
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);
        GPIO_ResetBits(GPIOG,GPIO_Pin_9);
        GPIO_ResetBits(GPIOD,GPIO_Pin_6);
        GPIO_SetBits(GPIOD,GPIO_Pin_3);
        show(temp2);
        Delay1ms(10);
    }
    else if(Nunber <  10)	
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);
        GPIO_ResetBits(GPIOG,GPIO_Pin_9);
        GPIO_ResetBits(GPIOD,GPIO_Pin_6);
        GPIO_SetBits(GPIOD,GPIO_Pin_3);
        show(Nunber);
        Delay1ms(10);
    }		
}


