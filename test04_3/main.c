#include<iocc2530.h>
#include <stdio.h>
#include"74LS164_8LED.h"

int counter_g = 0;
int i = 0;
void Delay()
{
    int y,x;
    
    for(y=1000;y>0;y--)
      for(x=30;x>0;x--);
}

void initTimer3(void)
{
    T3CTL = 0xA8;//32分频 溢出中断屏蔽开

    T3IE = 1;	   // 定时器3中断使能
    EA = 1;	       // 中断总允许

    T3CTL |= 0x10; // 开启中断
}

void Init32M()
{
   SLEEPCMD &=0xFB;//1111 1011 开启2个高频时钟源
   while(0==(SLEEPSTA & 0x40));// 0100 0000 等待32M稳定
   Delay();
   CLKCONCMD &=0xF8;//1111 1000 不分频输出
   CLKCONCMD &=0XBF;//1011 1111 设置32M作为系统主时钟
   while(CLKCONSTA & 0x40); //0100 0000 等待32M成功成为当前系统主时钟
}

void main(){
    Init32M();
    initTimer3();
    LS164_Cfg();
    P1SEL &= ~0x01;//P1_0设置为通用I/O口
    P1DIR |= 0X01;//配置P1_0位输出

   while(1);
}

/*
 *  Timer3 interrupt service function 
 */
#pragma vector = T3_VECTOR
__interrupt void Timer3_ISR(void)
{
    // ~1s
    if (++counter_g == 3906) {
        counter_g = 0;
        i++;
        
        if(i > 10)
            i = 0;
        P1_0 = ~ P1_0;
        LS164_BYTE(i);

    }
}