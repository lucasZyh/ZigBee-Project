#include<iocc2530.h>

void delay()
{
   int i,j;
     for(i=0;i<1000;i++)
         for(j=0;j<30;j++);
}

void main()
{
     P1DIR |=0X01;
     
     while(1)
     {
       P1_0=0;
       delay();
       P1_0=1;
       delay();
     }
}