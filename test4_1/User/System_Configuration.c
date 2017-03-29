#include"stm32f10x.h"
#include"System_Configuration.h"
ErrorStatus HSEStartUpStatus1;//�ж�HSE�Ƿ������־λ

/***************************************************************************
*��    �ƣ�RCC_Configuration
*��    �ܣ�ʱ��ģ���ʼ������
*��ڲ�������
*���ڲ�������
*˵    �������ð������õľ���Ϊ�ڲ����ⲿ�����ٻ���٣���PLL��Ƶ���ʱ��Ƶ��
*��    �����ú���Ŀǰ����Ϊ�ⲿ���پ���8M��PLL��Ƶ9������ϵͳʱ��Ϊ8M*9=72Mhz
****************************************************************************/
void RCC_Configuration(void)//ʱ������
{
  RCC_DeInit();//������ RCC �Ĵ�������Ϊȱʡֵ
  RCC_HSEConfig(RCC_HSE_ON);//�����ⲿ���پ��� HSE��
  HSEStartUpStatus1 = RCC_WaitForHSEStartUp();//�ȴ� HSE ����
  if(HSEStartUpStatus1 == SUCCESS) //HSE �����ȶ��Ҿ���
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//ʹ�ܻ���ʧ��Ԥȡָ����
    FLASH_SetLatency(FLASH_Latency_2); //���� FLASH �洢����ʱʱ��������
    RCC_HCLKConfig(RCC_SYSCLK_Div1);//���� AHB ʱ��,AHB ʱ�� = ϵͳʱ��
    RCC_PCLK2Config(RCC_HCLK_Div1);//���ø��� AHB ʱ��,APB2 ʱ�� = HCLK
    RCC_PCLK1Config(RCC_HCLK_Div2);//���õ��� AHB ʱ��,APB1 ʱ�� = HCLK / 2
    //PLL ������ʱ�� = HSE ʱ��Ƶ��,PLL ����ʱ�� x 9
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//���� PLL ʱ��Դ����Ƶϵ��
    RCC_PLLCmd(ENABLE);//ʹ�ܻ���ʧ�� PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//���ָ���� RCC ��־λ�������
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//����ϵͳʱ��
    while(RCC_GetSYSCLKSource() != 0x08);//��������ϵͳʱ�ӵ�ʱ��Դ
  }     
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);//ʹ��PA,PB,PC,PG,PF�˿�ʱ��
 
}
/***************************************************************************
*��    �ƣ�NVIC_Configuration
*��    �ܣ�Ƕ�������жϿ��ƣ����ø����жϵ����ȼ�
*��ڲ�������
*���ڲ�������
*˵    ������Ҫȷ�����жϵ�IRQ ͨ���������ȼ��������ȼ�
*��    ����
****************************************************************************/
void NVIC_Configuration(void)//Ƕ�������жϿ�����
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);               //ָ��������λ���� ����洢�� ��
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
}

/***************************************************************************
*��    �ƣ�GPIO_Configuration
*��    �ܣ�������Ӧ��IO����
*��ڲ�������
*���ڲ�������
*˵    ������Ҫ������Ӧʱ��Դ��GPIO_Pin,GPIO_Mode,GPIO_Speed
*��    ����
****************************************************************************/
void GPIO_Configuration(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
  //������ʼ������  
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//KEY0-KEY2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;       //PC0���ó��������룬 SW3 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
    
 //LED IO��ʼ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);				//�����趨������ʼ��
 GPIO_ResetBits(GPIOA,GPIO_Pin_8);		                //LED0
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);				 //LED7
 GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//LED1,LED2,LED3,LED4,LED5

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOG, &GPIO_InitStructure);				 //�����趨������ʼ
 GPIO_ResetBits(GPIOG, GPIO_Pin_6|GPIO_Pin_7);                  //LED6,LED7		


//Timer 2 ��ʱ��������������ܽ�PG12��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_Init(GPIOB,&GPIO_InitStructure); 
 
}


void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //  TIM_OCInitTypeDef  TIM_OCInitStructure ;
    TIM_DeInit(TIM2);                              //��λTIM2��ʱ��
    
    /* TIM2 clock enable [TIM2��ʱ������]*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* TIM2 configuration */
    TIM_TimeBaseStructure.TIM_Period = 7200;        // 16λ�Ĵ��������ֵ65535    
    TIM_TimeBaseStructure.TIM_Prescaler = 100;    //    72M/(100*7200)=100Hz--   10ms ��ʱ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // ʱ�ӷ�Ƶ  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM2 update pending flag[���TIM2����жϱ�־] */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* Enable TIM2 Update interrupt [TIM2����ж�����]*/
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  

    /* TIM2 enable counter [����tim2����]*/
   TIM_Cmd(TIM2, ENABLE);      //ʹ�ܶ�ʱ��  
}

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u16  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void SysTick_delay_init()	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//ѡ���ⲿʱ��  HCLK
	fac_us=SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*10000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
}								    
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 




/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the NVIC and Vector Table base address.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Configuration(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //ʹ��UART1ģ���ʱ��
  //************************����1��ʼ��***************************************
   //PA2 Ϊ USART1_TXD��������Ϊ���ܹܽ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
   //PA3 Ϊ USART1_RXD��������Ϊ��������ܽ�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  USART_InitStructure.USART_BaudRate = 115200;//���ڲ�������Ϊ115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1bitֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ���ʹ��

  USART_Init(USART1, &USART_InitStructure);//��ʼ��USART1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//����USART1�Ľ����ж�  
  USART_Cmd(USART1, ENABLE);          // Enable USART1
}