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
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);      //此种情况下， NVIC_IRQChannelSubPriority-15中优先级     
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;     //采用 SW3作为按键中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
  EXTI_InitTypeDef EXTI_InitStructure;   
  EXTI_ClearITPendingBit(EXTI_Line0);//清空中断标志
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0); //选择PC0为中断IO（）对应SW3    
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                 //选择中断线路0 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //设置为中断请求，非事件请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   //设置中断触发方式为下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;		   //外部中断使能
  EXTI_Init(&EXTI_InitStructure);
  
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
}


