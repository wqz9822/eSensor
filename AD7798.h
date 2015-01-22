/* ad7798 driver Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Driver is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

/* basic driver for ad7798 and ad7798, not all functions supported
   assumes a spi driver exists, and "SPI_Transfer" takes
   and returns a byte.  Also, due to the nature of the ad7798 triggering
   interrupts on the MISO line, it is important to keep the ad7798 selected
   to receive the interrupt most of the time.  Therefore, all of the below
   functions assume the ad7798 is selected already */
#include "SPI.h"
#include <avr/io.h>

#define AD7798SSTATUS_RDY       0x80
#define ad7798_read_status_ERROR     0x40
#define ad7798_read_status_NOREF     0x20
#define ad7798_read_status_IS_AD7798 0x08
#define ad7798_read_status_CHAN_MASK 0x07

uint8_t   ad7798_init_status;

#define ad7798_read_status_REG 0x0
#define AD7798_MODE_REG   0x1
#define AD7798_CONFIG_REG 0x2
#define AD7798_DATA_REG   0x3
#define AD7798_ID_REG     0x4
#define AD7798_IO_REG     0x5
#define AD7798_OFFSET_REG 0x6
#define AD7798_SCALE_REG  0x7



/* helper request function, assumes slaveselect is already set */
static inline void ad7798_comm(uint8_t reg, uint8_t read, uint8_t cont)

{
	
	CS_active;
	CS_inactive;
	SPI_Transfer((read ? 0x40 : 0x00) | (reg << 3) | (cont ? 0x04 : 0x00));
  
}

//----------------------------------------------//
void ad7798_reset(void)
{
 
	int i;
	CS_inactive;
	for(i = 0; i<4; i++) SPI_Transfer(0xff);
	CS_active;
}

//---------------------------------------//

uint8_t ad7798_read_status(void)
{
   ad7798_comm(ad7798_read_status_REG, 1, 0);
   return SPI_Transfer(0);
}


/* setting config, pick gain and chan from below,
   the rest are either zero or non-zero */
enum { AD7798_1_GAIN, AD7798_2_GAIN, AD7798_4_GAIN, AD7798_8_GAIN,
       AD7798_16_GAIN, AD7798_32_GAIN, AD7798_64_GAIN, AD7798_128_GAIN };
enum { AD7798_AIN1_CHAN, AD7798_AIN2_CHAN, AD7798_AIN3_CHAN,
       AD7798_AIN11_CHAN, AD7798_AVDD_CHAN };

inline void ad7798_write_config(uint8_t burnout, uint8_t unipolar, uint8_t gain,
                                uint8_t ref_det, uint8_t buf, uint8_t chan)
{
   ad7798_comm(AD7798_CONFIG_REG, 0, 1);
   SPI_Transfer((burnout ? 0x40 : 0x00) | (unipolar ? 0x20 : 0x00) | gain);
   SPI_Transfer((ref_det ? 0x20 : 0x00) | (buf ? 0x10 : 0x00) | chan);

}

uint16_t ad7798_read_config()
{
	uint16_t temp;
	ad7798_comm(AD7798_CONFIG_REG, 1, 1);
	temp = SPI_Transfer(0);
	temp <<=8;
	temp |=SPI_Transfer(0);
	CS_active;
	return temp;
}


/* return the contents of the OFFSET register */
int16_t ad7798_read_offset(void)
{
   ad7798_comm(AD7798_OFFSET_REG, 1, 0);
   int16_t val = 0;
   val |= (int16_t)SPI_Transfer(0) << 8;
   val |= (int16_t)SPI_Transfer(0);
   CS_active;
   return val;
}

/* return the contents of the FULL-SCALE register */
int16_t ad7798_read_scale(void)
{
   ad7798_comm(AD7798_SCALE_REG, 1, 0);
   int16_t val = 0;

   
   val |= (int16_t)SPI_Transfer(0) << 8;
   val |= (int16_t)SPI_Transfer(0);
   CS_active;
   return val;

}



/*  Setting the mode, pick one for mode and rate,
    psw is either zero or non-zero */
enum { AD7798_CONTINUOUS_CONVERSION_MODE = 0,  AD7798_SINGLE_CONVERSION_MODE,
       AD7798_IDLE_MODE, AD7798_POWERDOWN_MODE, AD7798_INTERNAL_OFFSET_CAL_MODE,
       AD7798_INTERNAL_SCALE_CAL_MODE,  AD7798_SYSTEM_OFFSET_CAL_MODE,
       AD7798_SYSTEM_SCALE_CAL_MODE};

enum { AD7798_470_HZ = 1, AD7798_242_HZ, AD7798_123_HZ, AD7798_62_HZ,
       AD7798_50_HZ, AD7798_39_HZ, AD7798_33_2_HZ, AD7798_19_6_HZ,
       AD7798_16_7_1_HZ, AD7798_16_7_2_HZ, AD7798_12_5_HZ, AD7798_10_HZ,
       AD7798_8_33_HZ, AD7798_6_25_HZ, AD7798_4_17_HZ };

void ad7798_set_mode(uint8_t mode, uint8_t psw, uint8_t rate)
{
    ad7798_comm(AD7798_MODE_REG, 0, 0);
    SPI_Transfer(mode << 5 | (psw ? 0x10 : 0x00));
    SPI_Transfer(rate);
	CS_active;
}

void ad7798_get_mode(uint8_t *mode, uint8_t *psw, uint8_t *rate)
{
    ad7798_comm(AD7798_MODE_REG, 1, 0);
    uint8_t val = SPI_Transfer(0);
    if(mode)
        *mode = val >> 5;
    if(psw)
        *psw = val & 0x10 ? 1 : 0;
    val = SPI_Transfer(0);
    if(rate)
        *rate = val;
    CS_active;
}


/* determine if data is ready to be read, could also be
   implemented by reading the status register */
inline int8_t ad7798_data_ready(void)
{
    return !(PINB & _BV(SPI_MISO));
}

/* request a read from the data register */
void __attribute__((always_inline)) ad7798_request_data(uint8_t continuous)
{
  
   ad7798_comm(AD7798_DATA_REG, 1, continuous);
}

/* read from data register, it should be previously requested
   from ad7798_request_data, the value is signed */
uint16_t __attribute__((always_inline)) ad7798_read_data(void)
{
   uint16_t val;
   val = SPI_Transfer(0);
   val <<= 8;
   val |= SPI_Transfer(0);
   
   return val ;
}

void ad7798_calibrate()
{
   int16_t off = ad7798_read_offset();

   /* cal */
   ad7798_set_mode(AD7798_INTERNAL_OFFSET_CAL_MODE, 0, AD7798_16_7_1_HZ);
   while(!ad7798_data_ready());

   off = ad7798_read_offset();
}

//------------------------------------------//
void ad7798_init()
{
    /*

    uint8_t mode,psw,rate;
    uint16_t offset,scale,config
    offset = ad7798_read_offset();
    scale = ad7798_read_scale();
    ad7798_get_mode(&mode,&psw,&rate);
    config = ad7798_read_config();
    */
   ad7798_reset();
   ad7798_init_status = ad7798_read_status();
   ad7798_write_config(0,1,AD7798_1_GAIN,0,1,AD7798_AIN1_CHAN);//no burnout,unipolar set,gain 1(2.5 ref),no ref_det,buf set)
   CS_active;
}

int32_t ad7798_Data()
{
  uint32_t temp_read;
  while(!ad7798_data_ready());
  SPI_Transfer(0x58);
  temp_read =  SPI_Transfer(0);
  temp_read <<= 8;
  temp_read |= SPI_Transfer(0);
  return temp_read - 0x8000;

}
