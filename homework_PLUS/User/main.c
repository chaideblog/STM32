/*****************************����*****************************/
#include"stm32f10x.h"
#include"System_Configuration.h"
#include"LCD.h"
#include"Seg.h"
#include <math.h>

/***************************��������***************************/
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

/***************************ȫ�ֱ���***************************/
u8 Key_Pressed_Cnt=0;
u8 Key_F=1,Key_B=1;
u32 Delay_1ms_Cnt=0;
char key[4][4];
int display[16] = {17,18,19,20,21,22,23,24,25,26,12,14,11,16,99,30};
char Key_val = 0, Disp_Key_val = 0;

/*****************************�ṹ��****************************/
struct io_port {                                           
  GPIO_TypeDef *GPIO_x;                  
  unsigned short GPIO_pin; 
};                      // 4*4�������  
static struct io_port key_output[4] = { 
{GPIOF, GPIO_Pin_6},    // 0��
{GPIOC, GPIO_Pin_10},   // 1��
{GPIOF, GPIO_Pin_7},    // 2��
{GPIOC, GPIO_Pin_13}};  // 3��
static struct io_port key_input[4] = { 
{GPIOC, GPIO_Pin_14},   // 0��
{GPIOE, GPIO_Pin_5},    // 1��
{GPIOC, GPIO_Pin_15},   // 2��
{GPIOE, GPIO_Pin_6} };  // 3��

/*****************************������****************************/
int main(void)
{
  /*-------------------------��������-------------------------*/
  
  char queue[20] = {'$','$','$','$','$','$','$','$','$','$'};
  int output = 0;
  int LCD_NUM = -1;
  
  /*--------------------------��ʼ��--------------------------*/
  CleanKeyNunber();               // �������λ����Ϣ key[i][j] = 0
  GPIO_Configuration();           // IO��ʼ��
  SEG_GPIO_Configuration();
  RCC_Configuration();            // ʱ������
  SysTick_delay_init();           // �趨SysTich Ƶ��Ϊ72M/72000=1ms
  LCM_IO_Init();
  Init_ST7567();
  ClearScreen();                  // clear all dots
  USART_Configuration(); //���ô���

  while(1){
    CleanKeyNunber();             // �������λ����Ϣ key[i][j] = 0
    Key_Scan();                   // ����ɨ�裬���ذ���λ����Ϣ key[i][j] = 1
    Key_val = TransferNumber();   // key[i][j] = 1����Ϊ0-15��������0xff
    if(Key_val != 0xff){
      LCD_NUM++;
      Disp_Key_val = Key_val;
      SerialPutChar(num2char(Disp_Key_val));
    }// end if
    CleanKeyNunber();             // �������λ����Ϣ key[i][j] = 0
    
    if(Disp_Key_val == 14){
      // �������ɨ�赽14����*��C��������
      LCD_NUM = -1;               // ��LCD��������
      clearLCD();
      cleanMyMatrix(queue, 10);
      continue;
    }else if(Disp_Key_val == 15){
      DisplayFont(1, LCD_NUM * 8 + 1, 30, FONT_8X16);
      queue[LCD_NUM] = Disp_Key_val;    // ������ѹջ
      output = computer(queue);
      displayNum(output, LCD_NUM + 1);
    }else if(Disp_Key_val > -1 & Disp_Key_val < 14){
      queue[LCD_NUM] = Disp_Key_val;    // ������ѹջ
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
  USART_SendData(USART1, c);                                  //��Ҫ���͵�������䵽���ͻ�����
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ����ͽ���
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
    Key_Pressed_Cnt++;//��⵽����
    if(Key_Pressed_Cnt==2)
      Key_Pressed_Cnt=0;
  }
  Key_B=Key_F;
}
*/
void Delay_1ms(u32 cnt)
{
  Delay_1ms_Cnt=cnt;
  while(Delay_1ms_Cnt); //�ȴ�����SysTick�ж�cnt�˺��˳�Delay_1ms����
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

void  Key_Scan(void)  //������ʱ���İ���ɨ�����
{	
  char i, j;  
  for(i = 0; i < 4; i++)             //i������ڣ������õ͵�ƽ 
  {   
    GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);       
    for(j = 0; j < 4; j++)            //j������ڣ���������ʱ��ͨ����Ϊ�͵�ƽ    
    {      
        if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
        {
          Delay1ms(10);            //��ʱȥ��
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
  while(cnt_1ms--)   //Ĭ��ϵͳ��ʱ��72M�������Ч��Լ1/8,��һ������ԼΪ1ms
  {
    i=8000;             
    while(i--);
  }
}

char TransferNumber(void)//�������룺����ʵ�����Ӳ������,�Ѱ�����λ����Ϣkey[i][j] = 1����Ϊ0-15����
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
  //����������ʼ������  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG, ENABLE);//ʹ��PA,PB,PC,PG,PF�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//KEY0-KEY2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PC0���ó����룬 KEY3
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //���а�����ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��
  
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE
  
  for(i = 0; i < 4; i++) 
     GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin); 
  
  
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