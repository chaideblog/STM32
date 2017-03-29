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
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);      //��������£� NVIC_IRQChannelSubPriority-15�����ȼ�     
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;     //���� SW3��Ϊ�����ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
  EXTI_InitTypeDef EXTI_InitStructure;   
  EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0); //ѡ��PC0Ϊ�ж�IO������ӦSW3    
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                 //ѡ���ж���·0 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //����Ϊ�ж����󣬷��¼�����
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   //�����жϴ�����ʽΪ�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;		   //�ⲿ�ж�ʹ��
  EXTI_Init(&EXTI_InitStructure);
  
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
}


