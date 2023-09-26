#include<iocc2530.h>

void delay()
{
   int i,j;
   for(i=0;i<1000;i++)
     for(j=0;j<30;j++);
}

void main(){
  
   P0SEL &= ~0x20;//P0_5设置为通用I/O口 1101 1111
   P0DIR &= ~0x20;//P0_5设置为输入
   P0INP &= ~0x20;//P0_5设置为上下拉
   P2INP &= ~0X20;//P0_5设置为上拉  
   
   //1.打开中断总允许
   EA=1;
   //2.打开P0口中断允许
   P0IE = 1;
   
   /*
   P1IE如果要设置为1，不能直接用P1IE = 1，不能直接使用位操作
   应该使用 IEN2 |= 0x10; // 0001 0000 相当于P1IE=1
   */

   //3.打开组内的某一位中断允许
   P0IEN |= 0x20;//P0_5中断打开 0010 0000
   //4.设置触发方式
   PICTL |=0x01;//把P0这一组配置成下降沿触发

   P1SEL &= ~0x01;//P1_0设置为通用I/O口
   P1DIR |= 0X01;//配置P1_0位输出

   while(1);
}

#pragma vector = P0INT_VECTOR
__interrupt void P0_ISR(void){
   if(P0IFG & 0x20)//判断P0_5是否发生中断
   {
      delay();
      if(0==P0_5){
         //说明确实是连接在P0_5的按钮触发了外部中断
         P1_0 ^=1;//异或操作
      }
   }
   //清除中断标志 
   P0IFG=0;
   P0IF=0;
}