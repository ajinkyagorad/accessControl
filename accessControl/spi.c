/*
 * spi.c
 *
 * Created: 01-05-2015 03:45:47 AM
 *  Author: Ajinkya
 */ 

#include "spi.h"
//reference : http://www.tinkerer.eu/AVRLib/SPI/


void spi_init()
// Initialize pins for spi communication
{
	DDR_SPI &= ~((1<<DD_MOSI)|(1<<DD_MISO)|(1<<DD_SS)|(1<<DD_SCK));
	// Define the following pins as output
	DDR_SPI |= ((1<<DD_MOSI)|(1<<DD_SS)|(1<<DD_SCK));
	
	SPCR = ((1<<SPE)|               // SPI Enable
	(0<<SPIE)|              // SPI Interupt Enable
	(0<<DORD)|              // Data Order (0:MSB first / 1:LSB first)
	(1<<MSTR)|              // Master/Slave select
	(0<<SPR1)|(1<<SPR0)|    // SPI Clock Rate
	(0<<CPOL)|              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
	(0<<CPHA));             // Clock Phase (0:leading / 1:trailing edge sampling)

	SPSR = (1<<SPI2X);              // Double Clock Rate
}

void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len)
// Shift full array through target device
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		SPDR = dataout[i];
		while((SPSR & (1<<SPIF))==0);
		datain[i] = SPDR;
	}
}

void spi_transmit_sync (uint8_t * dataout, uint8_t len)
// Shift full array to target device without receiving any byte
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		SPDR = dataout[i];
		while((SPSR & (1<<SPIF))==0);
	}
}

uint8_t spi_fast_shift (uint8_t data)
// Clocks only one byte to target device and returns the received one
{
	SPDR = data;
	while((SPSR & (1<<SPIF))==0);
	return SPDR;
}

uint8_t spi_read(void)
// Clocks only one byte(0xFF) to target device and returns the received one
{
	SPDR = 0xFF;
	while((SPSR & (1<<SPIF))==0);
	return SPDR;
}
void spi_write(uint8_t data)
{
	SPDR = data;
	while((SPSR & (1<<SPIF))==0);
}