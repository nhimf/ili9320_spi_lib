/*
 * Name: ili9320.h
 * Author: Maarten van Ingen
 * Copyright: 2013, Maarten van Ingen
 * License: See LICENSE file
*/

#ifndef GoL_ili9320_h
#define GoL_ili9320_h

#define MAX_X  240
#define MAX_Y  320

#define SPI_START   (0x70)              /* Start byte for SPI transfer        */
#define SPI_RD      (0x01)              /* WR bit 1 within start              */
#define SPI_WR      (0x00)              /* WR bit 0 within start              */
#define SPI_DATA    (0x02)              /* RS bit 1 within start byte         */
#define SPI_INDEX   (0x00)              /* RS bit 0 within start byte         */

#define DISPLAY_RESET_PIN   PB1
#define DISPLAY_RESET_DDR   DDRB
#define DISPLAY_RESET_PORT  PORTB

/* LCD colors */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0


void Display_WriteIndex ( uint8_t index );
void Display_WriteData ( uint16_t data );
void Display_WriteDataStart ( void);
void Display_WriteRegister ( uint16_t Display_Reg, uint16_t Display_RegValue );
uint16_t Display_ReadData(void);
uint16_t Display_ReadRegister( uint16_t Display_Reg);
void Display_Init(void);

void Display_Clear( uint16_t Color);
void Display_SetPoint( uint16_t Xpos, uint16_t Ypos, uint16_t point);
void Display_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );

#endif
