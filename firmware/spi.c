/*
 * Name: spi.c
 * Author: Maarten van Ingen
 * Copyright: 2013, Maarten van Ingen
 * License: See LICENSE file
 * Description: spi helper functions
*/

#include <stdio.h>
#include "spi.h"

// For now we assume all SPI slaves are active low on their CS pin
void SPI_SelectSlave ( )
{
    SPI_CSPORT &= ~( 1<<SPI_CSPIN );
}

void SPI_ReleaseSlave ( )
{
    SPI_CSPORT |= (1 << SPI_CSPIN );
}

void SPI_Init (void)
{
    SPI_CSDDR |= ( 1 << SPI_CSPIN );  // Set CS/SS pin to output mode. Note this is NOT the SS PIN on the ATmega

    DDRB |= (1<<PB2);
    SPI_ReleaseSlave ( );
    DDRB |= ( (1<<PB4) | (1<<PB5) | (1<<PB7) );	 // Set MOSI , SCK , and SS output

    /* Enable SPI:
     * Master
     * Set clock rate fck/4
     * Double SPI frequency (SPI2X)
     * @ 8MHz that is 4MHz
     */
    SPSR |= (1<<SPI2X);
    SPCR = ( (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA) );
}




void SPI_WriteByte ( unsigned char byte )
{
    SPDR = byte;					//Load byte to Data register
    while(!(SPSR & (1<<SPIF))); 	// Wait for transmission complete
}

unsigned char SPI_WriteReadByte ( unsigned char byte )
{
    PORTB |= (1<<PB2);
	SPDR = byte;					// Load byte to Data register
	while(!(SPSR & (1<<SPIF))); 	// Wait for transmission complete
	byte=SPDR;
    PORTB &= ~(1<<PB2);
	return byte;
}
