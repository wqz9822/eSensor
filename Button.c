#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "Display_Driver.h"


void Button_State(void)
{
    EIMSK |= (1<<PCIE0);    //使能外部中断PCIE0
    PCMSK0 |= (1<<PCINT7);    //使能PCINT7口(按键5)的电平变化中断
	SREG |= (1<<7);           //全局中断使能
    sei();                    //功能似乎同上，视频里有但是说明书里没有，保险起见我就写了
	DDRB  &= ~(1<<4);          //定义PB4为输入口，下同
	PORTB |= (1<<4);         //使能上拉电阻，下同
	DDRB  &= ~(1<<5);          
	PORTB |= (1<<5);
	DDRB  &= ~(1<<6);          
	PORTB |= (1<<6);
	DDRB  &= ~(1<<7);          
	PORTB |= (1<<7);        
	if (!(PINB & (1<<4)))     //判断按键是否按下
	{
	    LCD_clear();
		ButtonChoice = 4;
	}
	else if(!(PINB & (1<<5)))
	{
	    LCD_clear();
		ButtonChoice = 3;
	}
	else if(!(PINB & (1<<6)))
	{
	    LCD_clear();
		ButtonChoice = 2;
	}
	else if(!(PINB & (1<<7)))
	{
	    LCD_clear();
		ButtonChoice = 1;
	}
}

SIGNAL(SIG_PIN_CHANGE0)            //PCINT0中断（按键5）
{
    LCD_clear();
	ButtonChoice = 5;
}

