#include <avr/io.h>
#include <math.h>
#include "common.h"
#include "i2c.h"

//********************************************
//启动传输
//***********************************************
void s_transstart(void)
{
   DATA_SETOUTPUT;
   SCK_SETOUTPUT;
   
   DATA_1;
   SCK_0;
   delay_little();
   SCK_1;
   delay_little();
   DATA_0;
   delay_little();
   SCK_0;
   delay_little();
   SCK_1;
   delay_little();
   DATA_1;
   delay_little();
   SCK_0;
}

//********************************
//写入一个字节的命令，并且检查传感器是否正确接收了这个数据，返回值为0表示正确接收
//*******************************
char s_write_byte(unsigned char value)
{
	unsigned char i;
	char error;
	DATA_SETOUTPUT;
	SCK_SETOUTPUT;

	SCK_0;
	DATA_0;
	for(i=0;i<8;i++)  //发送8位数据，丛机将在上升沿读取数据
	{
		SCK_0;
		if(value&(0x80>>i))
		DATA_1;
		else
		DATA_0;
		delay_little();
		SCK_1;
		delay_little();
	}

	SCK_0;     //在接下来的上升沿读取从机发送的“已收到”信号。
	DATA_SETINPUT;
	delay_little();
	SCK_1;
	delay_little();
	error = (DATA_IN?1:0);
	delay_little();
	SCK_0;
	DATA_SETOUTPUT;
	return error;

}
//********************************************
//读一个字节的数据，并且向传感器发出一个字节的“已接收”信号
//********************************************
char s_read_byte(unsigned char ack)
{
	 unsigned char i,val=0;
 
	 DATA_SETINPUT;
	 SCK_SETOUTPUT;
 	 SCK_0;

	 for (i=0x80;i>0;i>>=1)       //接收8位数据
	 {
		  SCK_1;          
		  delay_little();
		  if (DATA_IN)
		   val=(val | i);      //read bit
		  SCK_0;
		  delay_little();
	 }
 
	 if(ack)
	  DATA_1;
	 else
	  DATA_0;
	 DATA_SETOUTPUT;  //发送一个ack位(0)，表示是否接收到数据
 
	 delay_little();
	 SCK_1;
	 delay_little();
	 return val;
}


//************************************************
// makes a measurement (humidity/temperature) with checksum
//测量一次温度/湿度，无校验
//参数：0表示温度，1表示湿度
//**************************************************
int s_measure(unsigned char mode) 
{
	 unsigned char a,b;
	 s_transstart();  //启动传输
	 switch(mode)    //发送开始测量命令
	 {
		  case 0 : s_write_byte(MEASURE_TEMP); break;
		  case 1 : s_write_byte(MEASURE_HUMI); break;
		  default : break;
	 } 
 
	 DATA_SETINPUT;
	 while(DATA_IN);   //等待检测完成
 
	 //读取两个字节的数据
	 a=s_read_byte(0);
	 b=s_read_byte(1);
 
	 return (((int)a)<<8)+b;
 
}
