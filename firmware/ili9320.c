/*
 * Name: ili9320.c
 * Author: Maarten van Ingen
 * Copyright: 2013, Maarten van Ingen
 * License: See LICENSE file
 * Description: ili9320 driver
 * Note: Current version includes code of the sample code which comes with the HY28A display
*/

#include <stdio.h>
#include "ili9320.h"
#include "spi.h"
#include <inttypes.h>
#include <util/delay.h>


// Write an Index to the display
void Display_WriteIndex(uint8_t index)
{
	SPI_SelectSlave ();
    
    /* SPI write data */
	SPI_WriteReadByte(SPI_START | SPI_WR | SPI_INDEX);   /* Write : RS = 0, RW = 0       */
	SPI_WriteReadByte(0);
	SPI_WriteReadByte(index);
    
	SPI_ReleaseSlave ();
}

// Write register data
void Display_WriteData( uint16_t data)
{
	SPI_SelectSlave ();
    
	SPI_WriteReadByte(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
	SPI_WriteReadByte((data >>   8));                    /* Write D8..D15                */
	SPI_WriteReadByte((data & 0xFF));                    /* Write D0..D7                 */
    
	SPI_ReleaseSlave ();
}

// Write start byte only
void Display_WriteDataStart(void)
{
    SPI_WriteReadByte(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
}

// Reads data from the display
uint16_t Display_ReadData(void)
{
	uint16_t value;
    
	SPI_SelectSlave ();
    
	SPI_WriteReadByte(SPI_START | SPI_RD | SPI_DATA);    /* Read: RS = 1, RW = 1         */
	SPI_WriteReadByte(0);                                /* Dummy read 1                 */
	value   = SPI_WriteReadByte(0);                      /* Read D8..D15                 */
	value <<= 8;
	value  |= SPI_WriteReadByte(0);                      /* Read D0..D7                  */
    
	SPI_ReleaseSlave ();
	
	return value;
}

// Writes data to the selected register
void Display_WriteRegister( uint16_t Display_Reg, uint16_t Display_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	Display_WriteIndex(Display_Reg);
	/* Write 16-bit Reg */
	Display_WriteData(Display_RegValue);
}

// Reads data from the selected register
uint16_t Display_ReadReg( uint16_t Display_Reg)
{
	uint16_t Display_RAM;
	
	/* Write 16-bit Index (then Read Reg) */
	Display_WriteIndex(Display_Reg);
	/* Read 16-bit Reg */
	Display_RAM = Display_ReadData();
    
	return Display_RAM;
}

// Sets cursor to the given address (x,y)
static void Display_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
	/* 0x9320 */
	Display_WriteRegister(0x0020, Xpos );
	Display_WriteRegister(0x0021, Ypos );
}

// Initializes the display. This function should be called first for getting anything to work
void Display_Init(void)
{
    // Reset LCD, this is very important to get anything working
    // Sequence: Reset pin of LCD: HIGH, delay, LOW, delay, HIGH, longer delay
    DISPLAY_RESET_DDR |= (1<<DISPLAY_RESET_PIN); // Set reset pin As Output
    DISPLAY_RESET_PORT |= (1<<DISPLAY_RESET_PIN);
    _delay_ms(2);
    
    // Reset controller
    DISPLAY_RESET_PORT &= ~(1<<DISPLAY_RESET_PIN);
    _delay_ms(2);
    DISPLAY_RESET_PORT |= (1<<DISPLAY_RESET_PIN);
    _delay_ms(15);

	
    
    uint16_t DeviceCode;
	
	//Display_Configuration();
	DeviceCode = Display_ReadReg(0x0000);		/* Read ID	*/
	/* Different driver IC initialization*/
	if( DeviceCode == 0x9320 || DeviceCode == 0x9300 )
	{
		Display_WriteRegister(0x00,0x0000);
		Display_WriteRegister(0x01,0x0100); /* Driver Output Contral */
		Display_WriteRegister(0x02,0x0700); /* LCD Driver Waveform Contral */
		Display_WriteRegister(0x03,0x1038); /* Set the scan mode */
		Display_WriteRegister(0x04,0x0000); /* Scalling Contral */
		Display_WriteRegister(0x08,0x0202); /* Display Contral 2 */
		Display_WriteRegister(0x09,0x0000); /* Display Contral 3 */
		Display_WriteRegister(0x0a,0x0000); /* Frame Cycle Contal */
		Display_WriteRegister(0x0c,(1<<0)); /* Extern Display Interface Contral 1 */
		Display_WriteRegister(0x0d,0x0000); /* Frame Maker Position */
		Display_WriteRegister(0x0f,0x0000); /* Extern Display Interface Contral 2 */
		_delay_ms(50);
		Display_WriteRegister(0x07,0x0101); /* Display Contral */
		_delay_ms(50);
		Display_WriteRegister(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4)); /* Power Control 1 */
		Display_WriteRegister(0x11,0x0007);                              /* Power Control 2 */
		Display_WriteRegister(0x12,(1<<8)|(1<<4)|(0<<0));                /* Power Control 3 */
		Display_WriteRegister(0x13,0x0b00);                              /* Power Control 4 */
		Display_WriteRegister(0x29,0x0000);                              /* Power Control 7 */
		Display_WriteRegister(0x2b,(1<<14)|(1<<4));
        
		Display_WriteRegister(0x50,0);       /* Set X Start */
		Display_WriteRegister(0x51,239);	    /* Set X End */
		Display_WriteRegister(0x52,0);	    /* Set Y Start */
		Display_WriteRegister(0x53,319);	    /* Set Y End */
		_delay_ms(50);
        
		Display_WriteRegister(0x60,0x2700); /* Driver Output Control */
		Display_WriteRegister(0x61,0x0001); /* Driver Output Control */
		Display_WriteRegister(0x6a,0x0000); /* Vertical Srcoll Control */
		
		Display_WriteRegister(0x80,0x0000); /* Display Position? Partial Display 1 */
		Display_WriteRegister(0x81,0x0000); /* RAM Address Start? Partial Display 1 */
		Display_WriteRegister(0x82,0x0000); /* RAM Address End-Partial Display 1 */
		Display_WriteRegister(0x83,0x0000); /* Displsy Position? Partial Display 2 */
		Display_WriteRegister(0x84,0x0000); /* RAM Address Start? Partial Display 2 */
		Display_WriteRegister(0x85,0x0000); /* RAM Address End? Partial Display 2 */
		
		Display_WriteRegister(0x90,(0<<7)|(16<<0)); /* Frame Cycle Contral */
		Display_WriteRegister(0x92,0x0000);         /* Panel Interface Contral 2 */
		Display_WriteRegister(0x93,0x0001);         /* Panel Interface Contral 3 */
		Display_WriteRegister(0x95,0x0110);         /* Frame Cycle Contral */
		Display_WriteRegister(0x97,(0<<8));
		Display_WriteRegister(0x98,0x0000);         /* Frame Cycle Contral */
		Display_WriteRegister(0x07,0x0133);
	}
    _delay_ms(100);   /* delay 50 ms */
}

// Clears the display with the selected color
void Display_Clear( uint16_t Color)
{
	uint32_t index=0;
	
	Display_SetCursor(0,0);
    
	Display_WriteIndex(0x0022);
    
 	SPI_SelectSlave ();
	Display_WriteDataStart();
    
	for( index = 0; index < (uint32_t)MAX_X * MAX_Y; index++ )
	{
        SPI_WriteReadByte((Color >>   8));      /* Write D8..D15                */
        SPI_WriteReadByte((Color & 0xFF));      /* Write D0..D7                 */

	}
	SPI_ReleaseSlave ();
}



// Converts BGR color scheme to the more usual RGB.
// RRRRRGGGGGGBBBBB To BBBBBGGGGGGRRRRR
static unsigned short Display_BGR2RGB( unsigned short color)
{
	unsigned short  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

// Reads the color of the pixel at position x,y
uint16_t Display_GetPoint( uint16_t Xpos, uint16_t Ypos)
{
	uint16_t dummy;
	
	Display_SetCursor(Xpos,Ypos);
    
	Display_WriteIndex(0x0022);
	
	dummy = Display_ReadData();   /* An empty read */
	dummy = Display_ReadData();
    
	return  Display_BGR2RGB( dummy );
}

// Sets the color of a pixel at position x,y
void Display_SetPoint( uint16_t Xpos, uint16_t Ypos, uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	Display_SetCursor(Xpos,Ypos);
	Display_WriteRegister(0x0022,point);
}

// Draws a line from x1,y1 to x2,y2 with the selected color

void Display_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    int16_t dx,dy;      /* The definition of the X Y axis increase the value of the variable */
    int16_t temp;
    
    if( x0 > x1 )
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;
    }
    if( y0 > y1 )
    {
		temp = y1;
		y1 = y0;
		y0 = temp;
    }
    
	dx = x1-x0;
	dy = y1-y0;
    
    if( dx == 0 )
    {
        do
        {
            Display_SetPoint(x0, y0, color);
            y0++;
        }
        while( y1 >= y0 );
		return;
    }
    if( dy == 0 )
    {
        do
        {
            Display_SetPoint(x0, y0, color);
            x0++;
        }
        while( x1 >= x0 );
		return;
    }
	
    if( dx > dy )
    {
	    temp = 2 * dy - dx;
        while( x0 != x1 )
        {
	        Display_SetPoint(x0,y0,color);
	        x0++;
	        if( temp > 0 )
	        {
	            y0++;
	            temp += 2 * dy - 2 * dx;
	 	    }
            else
            {
			    temp += 2 * dy;
			}
        }
        Display_SetPoint(x0,y0,color);
    }
    else
    {
	    temp = 2 * dx - dy;
        while( y0 != y1 )
        {
	 	    Display_SetPoint(x0,y0,color);
            y0++;
            if( temp > 0 )
            {
                x0++;
                temp+=2*dy-2*dx;
            }
            else
			{
                temp += 2 * dy;
			}
        }
        Display_SetPoint(x0,y0,color);
	}
}

