#include "Display_Driver.h"
#include "common.h"
#include <avr/io.h>
#include "Data_Convert.h"
#include "Button.h"
#include "adc.h"
#include <stdio.h>


typedef struct tSegment_Address
{
	unsigned char *reg_add; 
	unsigned char seg_bit;
} Segment_Address;
// Each Segment is accessed via the register plus the digit (0,7)

Segment_Address Seg_Adr[7][7];// 6 digits, 7 segment ie. a,b,c,d,e,f,g

//NOTE!! A[9] , 9 is the total number of elements, it range from [0] to [8]

 unsigned char Display_Mask[12][7]=
{
	{1,1,1,1,1,1,0},	// display pattern of 0 (a,b,c,d,e,f,g)
	{0,1,1,0,0,0,0},	// display pattern of 1
	{1,1,0,1,1,0,1},	// display pattern of 2
	{1,1,1,1,0,0,1},	// display pattern of 3
	{0,1,1,0,0,1,1},	// display pattern of 4
	{1,0,1,1,0,1,1},	// display pattern of 5
	{1,0,1,1,1,1,1},	// display pattern of 6
	{1,1,1,0,0,0,0},	// display pattern of 7
	{1,1,1,1,1,1,1},	// display pattern of 8
	{1,1,1,1,0,1,1}, 	// display pattern of 9	
	{1,0,0,1,1,1,0},    // display pattern of C
	{1,1,0,0,1,1,1}     // display pattern of P	
};

/************************************************************************/
// Local functions
/************************************************************************/
static void Seg_Adr_init()
{
    //1-a
		Seg_Adr[0][0].reg_add = 0xFB;
		Seg_Adr[0][0].seg_bit = BIT2;
	//1-b
		Seg_Adr[0][1].reg_add = 0xF6;
		Seg_Adr[0][1].seg_bit = BIT2;
	//1-c
		Seg_Adr[0][2].reg_add = 0xF1;
		Seg_Adr[0][2].seg_bit = BIT2;
	//1-d
	 	Seg_Adr[0][3].reg_add = 0xEC;
		Seg_Adr[0][3].seg_bit = BIT3;
	//1-e
		Seg_Adr[0][4].reg_add = 0xF1;
		Seg_Adr[0][4].seg_bit = BIT3;
	//1-f
		Seg_Adr[0][5].reg_add = 0xFB;
		Seg_Adr[0][5].seg_bit = BIT3;
	//1-g
		Seg_Adr[0][6].reg_add = 0xF6;
		Seg_Adr[0][6].seg_bit = BIT3;
	//2-a
		Seg_Adr[1][0].reg_add = 0xFB;
		Seg_Adr[1][0].seg_bit = BIT4;
	//2-b
		Seg_Adr[1][1].reg_add = 0xF6;
		Seg_Adr[1][1].seg_bit = BIT4;
	//2-c
		Seg_Adr[1][2].reg_add = 0xF1;
		Seg_Adr[1][2].seg_bit = BIT4;
	//2-d
		Seg_Adr[1][3].reg_add = 0xEC;
		Seg_Adr[1][3].seg_bit = BIT5;
	//2-e
		Seg_Adr[1][4].reg_add = 0xF1;
		Seg_Adr[1][4].seg_bit = BIT5;
	//2-f
		Seg_Adr[1][5].reg_add = 0xFB;
		Seg_Adr[1][5].seg_bit = BIT5;
	//2-g
		Seg_Adr[1][6].reg_add = 0xF6;
		Seg_Adr[1][6].seg_bit = BIT5;
	//3-a
		Seg_Adr[2][0].reg_add = 0XFC;
		Seg_Adr[2][0].seg_bit = BIT5;
	//3-b
		Seg_Adr[2][1].reg_add = 0xF7;
		Seg_Adr[2][1].seg_bit = BIT5;
	//3-c
		Seg_Adr[2][2].reg_add = 0xF2;
		Seg_Adr[2][2].seg_bit = BIT5;
	//3-d
		Seg_Adr[2][3].reg_add = 0xED;
		Seg_Adr[2][3].seg_bit = BIT4;
	//3-e
		Seg_Adr[2][4].reg_add = 0xF2;
		Seg_Adr[2][4].seg_bit = BIT4;
	//3-f
		Seg_Adr[2][5].reg_add = 0xFC;
		Seg_Adr[2][5].seg_bit = BIT4;
	//3-g
		Seg_Adr[2][6].reg_add = 0xF7;
		Seg_Adr[2][6].seg_bit = BIT4;
	//4-a
		Seg_Adr[3][0].reg_add = 0XFC;
		Seg_Adr[3][0].seg_bit = BIT3;
	//4-b
		Seg_Adr[3][1].reg_add = 0xF7;
		Seg_Adr[3][1].seg_bit = BIT3;
	//4-c
		Seg_Adr[3][2].reg_add = 0xF2;
		Seg_Adr[3][2].seg_bit = BIT3;
	//4-d
		Seg_Adr[3][3].reg_add = 0xED;
		Seg_Adr[3][3].seg_bit = BIT2;
	//4-e
		Seg_Adr[3][4].reg_add = 0xF2;
		Seg_Adr[3][4].seg_bit = BIT2;
	//4-f
		Seg_Adr[3][5].reg_add = 0xFC;
		Seg_Adr[3][5].seg_bit = BIT2;

	//4-g
		Seg_Adr[3][6].reg_add = 0xF7;
		Seg_Adr[3][6].seg_bit = BIT2;
	//5-a
		Seg_Adr[4][0].reg_add = 0XFC;
		Seg_Adr[4][0].seg_bit = BIT1;

	//5-b
		Seg_Adr[4][1].reg_add = 0xF7;
		Seg_Adr[4][1].seg_bit = BIT1;

	//5-c
		Seg_Adr[4][2].reg_add = 0xF2;
		Seg_Adr[4][2].seg_bit = BIT1;
	//5-d
		Seg_Adr[4][3].reg_add = 0xED;
		Seg_Adr[4][3].seg_bit = BIT0;
	//5-e
		Seg_Adr[4][4].reg_add = 0xF2;
		Seg_Adr[4][4].seg_bit = BIT0;
	//5-f
		Seg_Adr[4][5].reg_add = 0xFC;
		Seg_Adr[4][5].seg_bit = BIT0;
	//5-g
		Seg_Adr[4][6].reg_add = 0xF7;
		Seg_Adr[4][6].seg_bit = BIT0;
	//6-a
		Seg_Adr[5][0].reg_add = 0xFB;
		Seg_Adr[5][0].seg_bit = BIT7;
	//6-b
		Seg_Adr[5][1].reg_add = 0xF6;
		Seg_Adr[5][1].seg_bit = BIT7;
	//6-c
		Seg_Adr[5][2].reg_add = 0xF1;
		Seg_Adr[5][2].seg_bit = BIT7;
	//6-d
		Seg_Adr[5][3].reg_add = 0xEc;
		Seg_Adr[5][3].seg_bit = BIT6;
	//6-e
		Seg_Adr[5][4].reg_add = 0xF1;
		Seg_Adr[5][4].seg_bit = BIT6;
	//6-f
		Seg_Adr[5][5].reg_add = 0xFB;
		Seg_Adr[5][5].seg_bit = BIT6;
	//6-g
		Seg_Adr[5][6].reg_add = 0xF6;
		Seg_Adr[5][6].seg_bit = BIT6;
    //左一小数点
		Seg_Adr[6][0].reg_add = 0xEC;
		Seg_Adr[6][0].seg_bit = BIT7;
    //左二小数点
	    Seg_Adr[6][1].reg_add = 0xED;
		Seg_Adr[6][1].seg_bit = BIT1;
	//左三小数点
	    Seg_Adr[6][2].reg_add = 0xED;
		Seg_Adr[6][2].seg_bit = BIT3;
	//左四小数点
	    Seg_Adr[6][3].reg_add = 0xED;
		Seg_Adr[6][3].seg_bit = BIT5;
	//°
        Seg_Adr[6][4].reg_add = 0xEC;
		Seg_Adr[6][4].seg_bit = BIT2;
    //％
        Seg_Adr[6][5].reg_add = 0xED;
		Seg_Adr[6][5].seg_bit = BIT6;
}

/************************************************************************/
// Global functions
/************************************************************************/
void LCD_init()	
{
	LCDCRB = (0<<LCDCS)|(0<<LCD2B) | (3<<LCDMUX0)|(2<<LCDPM0);
	LCDFRR = (1<<LCDPS0) | (FRAME_32<<LCDCD0);   	
	LCDCRA = (1<<LCDEN) | (1<<LCDAB) | (0<<LCDIE) | (0<<LCDBL);
	LCDCCR = (7<<LCDCC1);
	Seg_Adr_init();
}


void LCD_clear(void)
{
    char i,j;
    for(j=0;j<6;j++)
	{
	  for(i=0;i<7;i++)
	  {
			  *(Seg_Adr[j][i].reg_add)=0;
		
	  }
    }
}



void put_digit(char c , char digit) //c stand for 0~9,c,p 最左位为第6位
{
    char i;
	for(i=0;i<7;i++)
	{
		if (Display_Mask[c][i] == 1)
		{
			*(Seg_Adr[5-digit][i].reg_add) |= Seg_Adr[5-digit][i].seg_bit;
		} 
		
	}
	
}

void put_string(char str[7])
{
	int i,j;
	char t;  // store the present digit

	j = 0; //position to the screen 0~5
    for(i=0;i<7;i++)
	{
		t = str[i];
		if((t >= 48) && (t <= 57))  //0~9
		{
			put_digit(t-48,j);  //该数在第j位
			j++;
		}
		else if ((t == 46) && (i >= 1) && (i <= 4))  // 小数点 ,且只使用左四个
		{
			*(Seg_Adr[6][i-1].reg_add) |= Seg_Adr[6][i-1].seg_bit;
		}
		else if (t == 'c') // c
		{
			put_digit(10,j);
			j++;
			*(Seg_Adr[6][4].reg_add) |= Seg_Adr[6][4].seg_bit;   //put o
		}
		else if (t == 'p') //p
		{
			put_digit(11,j);
			j++;
		}
		else if (t == '%') //%
		{
			*(Seg_Adr[6][5].reg_add) |= Seg_Adr[6][5].seg_bit;
		}
		else // wrong format
		{
			j++;
		}


	}  

}



//打印数据，并附上相应单位
void display_data(float data,char unit)

{
	char  str[7] ;

    sprintf(str,"%f",data);
	str[6] = unit;
	LCD_clear();
	put_string(str);
	
}




