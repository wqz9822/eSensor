#include <avr/io.h>
#include "Display_Driver.h"
#include "common.h"
#include "Data_Convert.h"



//***************************************************
//温湿度的计算和修正
//************************************************
void calc()
{
	 const float C1=-4.0;      // for 12 Bit
	 const float C2=+0.0405;     // for 12 Bit
	 const float C3=-0.0000028;     // for 12 Bit
	 const float T1=+0.01;      // for 14 Bit @ 5V
	 const float T2=+0.00008;     // for 14 Bit @ 5V
	 float rh=humidity;      // rh: Humidity [Ticks] 12 Bit
	 float t=temperature;     // t: Temperature [Ticks] 14 Bit
	 float rh_lin;        // rh_lin: Humidity linear
	 float rh_true;        // rh_true: Temperature compensated humidity
	 float t_C;         // t_C : Temperature
	 t_C=t*0.01 - 40;       //calc. temperature from ticks
	 rh_lin=C3*rh*rh + C2*rh + C1;    //calc. humidity from ticks to [%RH]
	 rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;  //calc. temperature compensated humidity [%RH]
	 if(rh_true>100)rh_true=100;    //cut if the value is outside of
	 if(rh_true<0.1)rh_true=0.1;    //the physical possible range
	 temperature=t_C;      //return temperature
	 humidity=rh_true;      //return humidity[%RH]
}

float calc_ch2o(int32_t  ch2o_read)
{
	return ((float)ch2o_read / (0xffff))*10.0;

}
float calc_co2(int32_t  co2_read)
{	
	return ((float)co2_read / (0x3ff))* 5000.0;
}

