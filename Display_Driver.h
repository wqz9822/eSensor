//*****************************************************************************
//
//  File........: Display_Driver.h
//
//  Author(s)...: George Wang
//
//  Target(s)...: ATmega169
//
//  Description.: Defines and prototypes for Display_Driver.c
//
//  Revisions...: 1.0
//

//
//  2013.4.16 - 0.10 - File created                                  - RM
//
//*****************************************************************************


/* definitions for Frame Rate - the defs match the req'd bit setting */
#define  FRAME_256 0
#define  FRAME_128 1
#define  FRAME_85  2
#define  FRAME_64  3
#define  FRAME_51  4
#define  FRAME_42  5
#define  FRAME_36  6
#define  FRAME_32  7 



/************************************************************************/
// Global functions
/************************************************************************/

void LCD_init();
void put_digit(char c , char digit);
void LCD_clear(void);
void put_string(char str[7]); //ÏÔÊ¾6¸öÊý
void display_data(float data,char unit);
