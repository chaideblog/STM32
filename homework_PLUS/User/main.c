/*****************************引用*****************************/
#include"stm32f10x.h"
#include"System_Configuration.h"
#include"LCD.h"
#include"Seg.h"
#include <math.h>

/***************************函数声明***************************/
void Delay_1ms(u32 cnt);
void SerialPutChar(u8 c);
void GPIO_Configuration(void);
void CleanKeyNunber(void);
void  Key_Scan(void);
void Delay1ms(unsigned int cnt_1ms);
char TransferNumber(void);
void clearLCD(void);
void cleanMyMatrix(char * matrix, int length);
int displayNum(int num, int LCD_NUM);
void moveMatrix(char * matrix, int length, int location);
void Serial_PutString(u8 *s);
void SerialPutChar(u8 c);
char num2char(int num);

/***************************全局变量***************************/
u8 Key_Pressed_Cnt=0;
u8 Key_F=1,Key_B=1;
u32 Delay_1ms_Cnt=0;
char key[4][4];
int display[16] = {17,18,19,20,21,22,23,24,25,26,12,14,11,16,99,30};
char Key_val = 0, Disp_Key_val = 0;

/*****************************结构体****************************/
struct io_port {                                           
  GPIO_TypeDef *GPIO_x;                  
  unsigned short GPIO_pin; 
};                      // 4*4矩阵键盘  
static struct io_port key_output[4] = { 
{GPIOF, GPIO_Pin_6},    // 0行
{GPIOC, GPIO_Pin_10},   // 1行
{GPIOF, GPIO_Pin_7},    // 2行
{GPIOC, GPIO_Pin_13}};  // 3行
static struct io_port key_input[4] = { 
{GPIOC, GPIO_Pin_14},   // 0列
{GPIOE, GPIO_Pin_5},    // 1列
{GPIOC, GPIO_Pin_15},   // 2列
{GPIOE, GPIO_Pin_6} };  // 3列

/*****************************主函数****************************/
int main(void)
{
  /*-------------------------变量声明-------------------------*/
  
  char queue[20] = {'$','$','$','$','$','$','$','$','$','$'};
  int output = 0;
  int LCD_NUM = -1;
  
  /*--------------------------初始化--------------------------*/
  CleanKeyNunber();               // 清除按键位置信息 key[i][j] = 0
  GPIO_Configuration();           // IO初始化
  SEG_GPIO_Configuration();
  RCC_Configuration();            // 时钟配置
  SysTick_delay_init();           // 设定SysTich 频率为72M/72000=1ms
  LCM_IO_Init();
  Init_ST7567();
  ClearScreen();                  // clear all dots
  USART_Configuration(); //配置串口

  while(1){
    CleanKeyNunber();             // 清除按键位置信息 key[i][j] = 0
    Key_Scan();                   // 按键扫描，返回按键位置信息 key[i][j] = 1
    Key_val = TransferNumber();   // key[i][j] = 1换算为0-15的数，或0xff
    if(Key_val != 0xff){
      LCD_NUM++;
      Disp_Key_val = Key_val;
      SerialPutChar(num2char(Disp_Key_val));
    }// end if
    CleanKeyNunber();             // 清除按键位置信息 key[i][j] = 0
    
    if(Disp_Key_val == 14){
      // 如果键盘扫描到14，即*（C），清零
      LCD_NUM = -1;               // 将LCD计数重置
      clearLCD();
      cleanMyMatrix(queue, 10);
      continue;
    }else if(Disp_Key_val == 15){
      DisplayFont(1, LCD_NUM * 8 + 1, 30, FONT_8X16);
      queue[LCD_NUM] = Disp_Key_val;    // 将数字压栈
      output = computer(queue);
      displayNum(output, LCD_NUM + 1);
    }else if(Disp_Key_val > -1 & Disp_Key_val < 14){
      queue[LCD_NUM] = Disp_Key_val;    // 将数字压栈
      DisplayFont(1, LCD_NUM * 8 + 1, display[Disp_Key_val], FONT_8X16);
    }// end if-else
    
    Disp_Key_val = 0xff;
  }// end while
}// end main

char num2char(int num)
{
    if(num == 0)
      return('0');
    else if(num == 1)
      return('1');
    else if(num == 2)
      return('2');
    else if(num == 3)
      return('3');
    else if(num == 4)
      return('4');
    else if(num == 5)
      return('5');
    else if(num == 6)
      return('6');
    else if(num == 7)
      return('7');
    else if(num == 8)
      return('8');
    else if(num == 9)
      return('9');
    else if(num == 10)
      return('+');
    else if(num == 11)
      return('-');
    else if(num == 12)
      return('*');
    else if(num == 13)
      return('/');
    else if(num == 14)
      return('C');
    else if(num == 15)
      return('=');
    else 
      return('$');
}

  
void SerialPutChar(u8 c)
{
  USART_SendData(USART1, c);                                  //简要发送的数据填充到发送缓冲区
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//等待发送结束
}


int displayNum(int num, int LCD_NUM){  
  char everyBit[10] = {'$','$','$','$','$','$','$','$','$','$'};
  int i = 0;
  
  if(num == 0){
    DisplayFont(1, LCD_NUM * 8 + 1, 17, FONT_8X16);
      SerialPutChar(num2char(everyBit[i]));
      return(++LCD_NUM);
  }

  for(i = 0; i < 10; i++){
    everyBit[i] = num / (int)pow(10, 10 - i -1);
    num %= (int)pow(10, 10 - i - 1);
  }// end for
  
  while(everyBit[0] == 0){
    moveMatrix(everyBit, 10, 0);
  }// end while
  
  for(i = 0; i < 10; i++){
    if(everyBit[i] != '$'){
      DisplayFont(1, LCD_NUM * 8 + 1, display[everyBit[i]], FONT_8X16);
      SerialPutChar(num2char(everyBit[i]));
      LCD_NUM++;
    }else{
      break;
    }// end if-else
  }// end for
 
  return(LCD_NUM);
}// end displayNUM

int computer(char queue[20]){
  int i = 0, j = 0;
  char operation[10] = {'$','$','$','$','$','$','$','$','$','$'};
  int sum = 0;
  char number[10] = {'$','$','$','$','$','$','$','$','$','$'};
  char temp[10] = {'$','$','$','$','$','$','$','$','$','$'};
  int NUM = 0;
  
  for(i = 0, j = 0; i < 20; i++, j++){
    if(queue[i] < 10 & queue[i] > -1)
      temp[j] = queue[i];
    else if(queue[i] == 10 | queue[i] == 11 | queue[i] == 12 | queue[i] == 13 | queue[i] == 15){
      j = -1;
      number[NUM] = matrix2num(temp);
      operation[NUM] = queue[i];
      cleanMyMatrix(temp, 10);
      NUM++;
    }// end if-else
  }// end for
  
  for(i = 0; i < NUM; i++){
    // multiply
    if(operation[i] == 12){
      number[i] = number[i] * number[i + 1];
      moveMatrix(number, 10, i + 1);
      moveMatrix(operation, 10, i);
      i--;
    }// end if
    
    // dicision
    if(operation[i] == 13){
      number[i] = number[i] / number[i + 1];
      moveMatrix(number, 10, i + 1);
      moveMatrix(operation, 10, i);
      i--;
    }// end if
  }// end for
  
  for(i = 0; i < NUM; i++){
    // add
    if(operation[i] == 10){
      number[i] = number[i] + number[i + 1];
      moveMatrix(number, 10, i + 1);
      moveMatrix(operation, 10, i);
      i--;
    }// end if
    
    if(operation[i] == 11){
      number[i] = number[i] - number[i + 1];
      moveMatrix(number, 10, i + 1);
      moveMatrix(operation, 10, i);
      i--;
    }// end if
  }// end for

  if(operation[0] == 15){
    sum = number[0];
  }// end for
  
  return(sum);
}// end computer

void moveMatrix(char * matrix, int length, int location){
  int num = 0;
  
  for(num = location; num < length - 1; num++){
    *(matrix + num) = *(matrix + num + 1);
  }// end for
  
  *(matrix + length - 1) =  '$';
}// end moveMatrix

int matrix2num(char matrix[10]){
  int i = 0, num = 0;
  int length = 0;
  
  for(i = 0; i < 10; i++){
    if(matrix[i] == '$'){
      length = i;
      break;
    }// end if
  }// end for
  
  for(i = 0; i < 10; i++){
    if(matrix[i] != '$'){
      num += matrix[i] * (int)pow(10, length - i -1);
    }else{
      break;
    }// end if-else
  }// end for
  return(num);
}// end matrix

void cleanMyMatrix(char * matrix, int length){
  int num = 0;
  
  for(num = 0; num < length; num++){
    *(matrix + num) = '$';
  }// end for
}// end cleanMyMatrix

void clearLCD(void)
{
  int i,j;
  for(i=1; i<9; i++)
    for(j = 0; j < 16; j++)
      DisplayFont(i, j*8+1, 0, FONT_8X16);
}

/*
void Key_Scan(void)
{
 
  Key_F=KEY2;
  if((Key_F==0)&&(Key_B==1))
  {
    Key_Pressed_Cnt++;//检测到按键
    if(Key_Pressed_Cnt==2)
      Key_Pressed_Cnt=0;
  }
  Key_B=Key_F;
}
*/
void Delay_1ms(u32 cnt)
{
  Delay_1ms_Cnt=cnt;
  while(Delay_1ms_Cnt); //等待进入SysTick中断cnt此后，退出Delay_1ms函数
}
/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/

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

 void Delay1ms(unsigned int cnt_1ms)
{
  unsigned int i;
  while(cnt_1ms--)   //默认系统主时钟72M，反汇编效率约1/8,故一个周期约为1ms
  {
    i=8000;             
    while(i--);
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