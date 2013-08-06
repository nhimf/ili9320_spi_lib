/*
 * Name: spi.h
 * Author: Maarten van Ingen
 * Copyright: 2013, Maarten van Ingen
 * License: See LICENSE file
*/

#ifndef GoL_spi_h
#define GoL_spi_h

#include <avr/io.h>

#define SPI_CSPIN PB0
#define SPI_CSDDR DDRB
#define SPI_CSPORT PORTB


void SPI_SelectSlave ( );
void SPI_ReleaseSlave ( );
void SPI_Init (void);
void SPI_WriteByte ( unsigned char byte );
unsigned char SPI_WriteReadByte ( unsigned char byte );

#endif
