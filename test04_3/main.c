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
    T3CTL = 0xA8;//32��Ƶ ����ж����ο�

    T3IE = 1;	   // ��ʱ��3�ж�ʹ��
    EA = 1;	       // �ж�������

    T3CTL |= 0x10; // �����ж�
}

void Init32M()
{
   SLEEPCMD &=0xFB;//1111 1011 ����2����Ƶʱ��Դ
   while(0==(SLEEPSTA & 0x40));// 0100 0000 �ȴ�32M�ȶ�
   Delay();
   CLKCONCMD &=0xF8;//1111 1000 ����Ƶ���
   CLKCONCMD &=0XBF;//1011 1111 ����32M��Ϊϵͳ��ʱ��
   while(CLKCONSTA & 0x40); //0100 0000 �ȴ�32M�ɹ���Ϊ��ǰϵͳ��ʱ��
}

void main(){
    Init32M();
    initTimer3();
    LS164_Cfg();
    P1SEL &= ~0x01;//P1_0����Ϊͨ��I/O��
    P1DIR |= 0X01;//����P1_0λ���

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