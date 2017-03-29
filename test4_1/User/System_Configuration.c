#include"stm32f10x.h"
#include"System_Configuration.h"
ErrorStatus HSEStartUpStatus1;//判断HSE是否起振标志位

/***************************************************************************
*名    称：RCC_Configuration
*功    能：时钟模块初始化程序
*入口参数：无
*出口参数：无
*说    明：设置板子所用的晶振为内部或外部，高速或低速，及PLL倍频后的时钟频率
*范    例：该函数目前配置为外部高速晶振8M，PLL倍频9，所以系统时钟为8M*9=72Mhz
****************************************************************************/
void RCC_Configuration(void)//时钟设置
{
  RCC_DeInit();//将外设 RCC 寄存器重设为缺省值
  RCC_HSEConfig(RCC_HSE_ON);//设置外部高速晶振（ HSE）
  HSEStartUpStatus1 = RCC_WaitForHSEStartUp();//等待 HSE 起振
  if(HSEStartUpStatus1 == SUCCESS) //HSE 晶振稳定且就绪
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//使能或者失能预取指缓存
    FLASH_SetLatency(FLASH_Latency_2); //设置 FLASH 存储器延时时钟周期数
    RCC_HCLKConfig(RCC_SYSCLK_Div1);//设置 AHB 时钟,AHB 时钟 = 系统时钟
    RCC_PCLK2Config(RCC_HCLK_Div1);//设置高速 AHB 时钟,APB2 时钟 = HCLK
    RCC_PCLK1Config(RCC_HCLK_Div2);//设置低速 AHB 时钟,APB1 时钟 = HCLK / 2
    //PLL 的输入时钟 = HSE 时钟频率,PLL 输入时钟 x 9
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//设置 PLL 时钟源及倍频系数
    RCC_PLLCmd(ENABLE);//使能或者失能 PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//检查指定的 RCC 标志位设置与否
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//设置系统时钟
    while(RCC_GetSYSCLKSource() != 0x08);//返回用作系统时钟的时钟源
  }     
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);//使能PA,PB,PC,PG,PF端口时钟
 
}
/***************************************************************************
*名    称：NVIC_Configuration
*功    能：嵌套向量中断控制，设置各个中断的优先级
*入口参数：无
*出口参数：无
*说    明：需要确定该中断的IRQ 通道，及优先级，从优先级
*范    例：
****************************************************************************/
void NVIC_Configuration(void)//嵌套向量中断控制器
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);               //指定向量表位置在 程序存储器 中
  
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
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//KEY0-KEY2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;       //PC0设置成上拉输入， SW3 
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


//Timer 2 定时器方波测试输出管脚PG12初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_Init(GPIOB,&GPIO_InitStructure); 
 
}


void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //  TIM_OCInitTypeDef  TIM_OCInitStructure ;
    TIM_DeInit(TIM2);                              //复位TIM2定时器
    
    /* TIM2 clock enable [TIM2定时器允许]*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* TIM2 configuration */
    TIM_TimeBaseStructure.TIM_Period = 7200;        // 16位寄存器，最大值65535    
    TIM_TimeBaseStructure.TIM_Prescaler = 100;    //    72M/(100*7200)=100Hz--   10ms 定时器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM2 update pending flag[清除TIM2溢出中断标志] */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* Enable TIM2 Update interrupt [TIM2溢出中断允许]*/
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  

    /* TIM2 enable counter [允许tim2计数]*/
   TIM_Cmd(TIM2, ENABLE);      //使能定时器  
}

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u16  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void SysTick_delay_init()	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//选择外部时钟  HCLK
	fac_us=SystemCoreClock/8000000;	//为系统时钟的1/8  
	fac_ms=(u16)fac_us*10000;//非ucos下,代表每个ms需要的systick时钟数   
}								    
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
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
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //使能UART1模块的时钟
  //************************串口1初始化***************************************
   //PA2 为 USART1_TXD，需配置为功能管脚输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
   //PA3 为 USART1_RXD，需配置为功能输入管脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  USART_InitStructure.USART_BaudRate = 115200;//串口波特率设为115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1bit停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发均使能

  USART_Init(USART1, &USART_InitStructure);//初始化USART1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启USART1的接收中断  
  USART_Cmd(USART1, ENABLE);          // Enable USART1
}