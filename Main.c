#include <avr/io.h>
#include <stdlib.h> 
#include "Display_Driver.h"
#include "common.h"
#include "Data_Convert.h"
#include "Button.h"
#include "adc.h"
#include "SPI.h"
#include "i2c.h"
#include "ad7798.h"


int32_t  ch2o_read = 0;    //甲醛传感器测量值

int i,j;
int first_loop = 1, loop_num = 0;
 

void main(void)
{        
	
//=======================初始化====================================//
 	ButtonChoice = 3;               //按键功能选择变量
	LCD_init();
	ADC_init();
    SPI_init(); 
    ad7798_init();
 //==========================Main Loop===================================//
	 
	while(1)
	{     
		Button_State();                      //按键状态判断
		switch(ButtonChoice)           //按键功能定义
		{
			case 5:                   //关闭
			{
				LCD_clear();
				break;
			}
			case 4:                  //甲醛
			{
				
		    	if (first_loop)
				{

					CS_active;
					CS_inactive;
					first_loop = 0;
                }  
                
				
				if (loop_num < 8)
				{
					ch2o_read += ad7798_Data();
					loop_num++;
				}
				else
				{
					ch2o_read = ch2o_read >>3;  //average for better acuracy
					display_ppm_level(calc_ch2o_pm(ch2o_read));
					loop_num = 0;
					ch2o_read = 0;
				}

		        break;
			}

			case 3:                  //CO2
			{
				co2_read = ADC_read();
				display_ppm_level(calc_co2_pm(co2_read));
					 
				break;
			}
			case 2:                  //湿度
			{
				temp = s_measure(TEMP);
				humi = s_measure(HUMI);
				calc(&temp,&humi);
				//Put_SignRH();
				break;
			}
			case 1:                  //温度
			{
				temp = s_measure(TEMP);
				humi = s_measure(HUMI);
				calc(&temp,&humi);
				//Put_SignC();
				//Put_SignDel();
				break;
			}
			case 0:                  //初始状态定义
			{
				LCD_clear();
				break;
			}
			
		}
	}
}
