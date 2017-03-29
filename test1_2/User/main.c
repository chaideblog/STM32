#include"stm32f10x.h"
#include"Seg.h"

// 4*4矩阵键盘  
struct io_port {                                           
  GPIO_TypeDef *GPIO_x;                  
  unsigned short GPIO_pin; 
};           
static struct io_port key_output[4] = { 
{GPIOF, GPIO_Pin_6},      //0行
{GPIOC, GPIO_Pin_10},     //1行
{GPIOF, GPIO_Pin_7},      //2行
{GPIOC, GPIO_Pin_13}};    //3行
static struct io_port key_input[4] = { 
{GPIOC, GPIO_Pin_14},     //0列
{GPIOE, GPIO_Pin_5},      //1列
{GPIOC, GPIO_Pin_15},     //2列
{GPIOE, GPIO_Pin_6} };    //3列

char key[4][4]; 

void GPIO_Configuration(void);  
void CleanKeyNunber(void);      
void Key_Scan(void);           
void Delay1ms(unsigned int cnt_1ms);
char TransferNumber(void);      

int main(void)
{
  char Key_val,Disp_Key_val;
  CleanKeyNunber();         //清除按键位置信息 key[i][j] = 0
  GPIO_Configuration();     //IO初始化
  SEG_GPIO_Configuration();
	
  while(1)
  {
    Key_Scan();               //按键扫描，返回按键位置信息 key[i][j] = 1
    Key_val=TransferNumber(); //key[i][j] = 1换算为0-15的数，或0xff
    if(Key_val!=0xff)
    {
      Disp_Key_val=Key_val;
    }    
    ShowNunber(Disp_Key_val); //显示键值 0-15  
    CleanKeyNunber();	      //清除按键位置信息 key[i][j] = 0
  }
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
  char i;
   GPIO_InitTypeDef  GPIO_InitStructure;
  //独立按键初始化函数  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG, ENABLE);//使能PA,PB,PC,PG,PF端口时钟
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//KEY0-KEY2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PC0设置成输入， KEY3
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //阵列按键初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化
  
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE
  
  for(i = 0; i < 4; i++) 
     GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin); 
  
  
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

 void Delay1ms(unsigned int cnt_1ms)
{
  unsigned int i;
  while(cnt_1ms--)   //默认系统主时钟72M，反汇编效率约1/8,故一个周期约为1ms
  {
    i=8000;             
    while(i--);
  }
}
void CleanKeyNunber(void)
{
  uint8_t i,j;
  for(i = 0 ; i < 4; i ++)
    for(j = 0 ; j < 4; j ++)		
        key[i][j] = 0;
}
void  Key_Scan(void)  //采用延时法的按键扫描程序
{	
  char i, j;  
  for(i = 0; i < 4; i++)             //i是输出口，依次置低电平 
  {   
    GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);       
    for(j = 0; j < 4; j++)            //j是输入口，当键按下时导通被置为低电平    
    {      
        if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
        {
          Delay1ms(10);            //延时去抖
          if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
          {
            while(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0);
            key[i][j] = 1; 
          }                
        }
        else
                key[i][j] = 0;
    }
    GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
  }
}
char TransferNumber(void)//按键译码：根据实验箱的硬件配置,把按键的位置信息key[i][j] = 1换算为0-15的数
{
  char Key_Value;
    if(key[0][0] == 1)
            Key_Value = 14;
    else if(key[0][1] == 1)
            Key_Value = 7;
    else if(key[0][2] == 1)
            Key_Value = 4 ;
    else if(key[0][3] == 1)
            Key_Value = 1;
    else if(key[1][0] == 1)
            Key_Value = 0;
    else if(key[1][1] == 1)
            Key_Value = 8;
    else if(key[1][2] == 1)
            Key_Value = 5;
    else if(key[1][3] == 1)
            Key_Value = 2;
    else if(key[2][0] == 1)
            Key_Value = 15 ;
    else if(key[2][1] == 1)
            Key_Value = 9 ;
    else if(key[2][2] == 1)
            Key_Value = 6;
    else if(key[2][3] == 1)
            Key_Value = 3;
    else if(key[3][0] == 1)
            Key_Value = 13;
    else if(key[3][1] == 1)
            Key_Value = 12;
    else if(key[3][2] == 1)
            Key_Value = 11;
    else if(key[3][3] == 1)
            Key_Value = 10;
    else 
            Key_Value = 0xff;
    return Key_Value;	
}