#include<iocc2530.h>

char ch;
void delayus()
{
    char k=63;
    while(k--);
}

void Init32M(){
   SLEEPCMD &= 0xFB;//1111 1011 开启2个高频时钟源
   while(0==(SLEEPSTA & 0x40));// 0100 0000 等待32M稳定
   delayus();
   CLKCONCMD &= 0xF8;//1111 1000 不分频输出
   CLKCONCMD &= 0XBF;//1011 1111 设置32M作为系统主时钟
   while(CLKCONSTA & 0x40); //0100 0000 等待32M成功成为当前系统主时钟
    SLEEPCMD |= 0x40;
}

void USART_Init(){
  //串口0的备用位置1配置成波特率9600
  PERCFG &= 0xFE;       //1111 1110 选中串口0的的备用位置1

  P0SEL  |= 0x0C;       //0000 1100 P0_2 p0_3为片上外设功能
  
  U0CSR |= 0Xc0;
  
  U0GCR = 8;
  U0BAUD= 59;

  EA = 1;
  URX0IE = 1;
}

void main(){
  Init32M();
  USART_Init();
  while(1);
}

#pragma vector=URX0_VECTOR
__interrupt void USART0_INT(void)
{
    
    URX0IF=0;//串口0来数据的标志位，硬件会置1，我们软件要清0
    ch=U0DBUF;//从接受寄存器里取字节存入变量ch
    
    U0DBUF=ch;//把变量ch里的值赋给串口0发送数据寄存器
    while(0==UTX0IF);//等待发送完成
    UTX0IF=0;//发送完成标志位，硬件置1，软件清0
}