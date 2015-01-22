#include <avr/io.h>
#include <compat/deprecated.h>

//Dataflash macro definitions
#define CS_active	sbi(PORTB,0)
#define CS_inactive	cbi(PORTB,0)

#define SPI_PORT PORTB
#define SPI_DDR DDRB

#define SPI_CS   0  
#define SPI_SCK  1
#define SPI_MOSI 2
#define SPI_MISO 3

void SPI_init(void);
unsigned char SPI_Transfer (unsigned char output);

