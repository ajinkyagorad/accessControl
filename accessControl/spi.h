/*
 * spi.h
 *
 * Created: 01-05-2015 03:45:56 AM
 *  Author: Ajinkya
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "include.h"




#define PORT_SPI    PORTB
#define DDR_SPI     DDRB
#define DD_MISO     6
#define DD_MOSI     5
#define DD_SS       4
#define DD_SCK      7

#define SPI_PORT	PORT_SPI
#define SPI_DDR		DDR_SPI
#define SPI_CS		DD_SS

//spi library reference : http://www.tinkerer.eu/AVRLib/SPI/
extern void spi_init();
extern void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len);
extern void spi_transmit_sync (uint8_t * dataout, uint8_t len);
extern uint8_t spi_fast_shift (uint8_t data);
uint8_t spi_read(void);
void spi_write(uint8_t data);

#endif /* SPI_H_ */