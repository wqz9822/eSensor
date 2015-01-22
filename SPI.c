#include <avr/io.h>
#include "SPI.h"


unsigned char SPI_Transfer (unsigned char output)
{
	unsigned char input;
	
	SPDR = output;							//put byte 'output' in SPI data register
	while(!(SPSR & 0x80));					//wait for transfer complete, poll SPIF-flag
	input = SPDR;							//read value in SPI data reg.
	
	return input;							//return the byte clocked in from SPI slave
}	


void SPI_init(void) //初始化SPI主机模式
{   
    
    PORTB |= (1<<SPI_MOSI)|(1<<SPI_CS);
	DDRB = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_CS); //设为输出
    SPCR = (1<<SPE)|(1<<MSTR)|(1<CPOL)|(1<CPHA)|(1<<SPR0)|(1<<SPR1);	
}	


