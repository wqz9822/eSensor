#include<avr/io.h>
#include<avr/interrupt.h>
#include"adc.h"
#include <compat/deprecated.h>
#include "common.h"

//ADC初始化
void ADC_init(void)
{
     ADMUX |= (0<<REFS1)|(1<<REFS0);
     ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //12mhz / 128 =93.75khz
	 
}


int32_t ADC_read(void)
{
    char i;
    int32_t ADC_temp;
    int32_t ADC_readout = 0;
    
   

    sbi(ADCSRA, ADEN);     // Enable the ADC

    //do a dummy readout first
    ADCSRA |= (1<<ADSC);        // do single conversion
    while(!(ADCSRA & 0x10));    // wait for conversion done, ADIF flag active
        
    for(i=0;i<8;i++)            // do the ADC conversion 8 times for better accuracy 
    {
        ADCSRA |= (1<<ADSC);        // do single conversion
        while(!(ADCSRA & 0x10));    // wait for conversion done, ADIF flag active
        
        ADC_temp = ADCL;            // read out ADCL register
        ADC_temp += (ADCH << 8);    // read out ADCH register        

        ADC_readout += ADC_temp;      // accumulate result (8 samples) for later averaging
    }

    ADC_readout = ADC_readout >> 3;     // average the 8 samples

    cbi(ADCSRA, ADEN);      // disable the ADC
    
    return ADC_readout;
}

