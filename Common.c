#include "common.h"
//*******************************
//延时若干毫秒
//********************************
void delay_ms(int t)
{
   int i,j;
  for(i=0;i<t;i++)
     for(j=0;j<1141;j++);
}
//*******************************
//短延时
//*******************************
void delay_little(void)
{
 asm("nop");
 asm("nop");
 asm("nop");
 asm("nop");
 asm("nop");
 asm("nop");
}
