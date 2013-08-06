/*
 * Name: main.c
 * Author: Maarten van Ingen
 * Copyright: 2013, Maarten van Ingen
 * License: See LICENSE file
 * Description: Example main.c for use of ili9320 driver
*/

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "ili9320.h"

int main(void)
{
    SPI_Init();
    Display_Init();

    for(;;)
    {
        Display_Clear (White);
        _delay_ms ( 1000 );
        Display_Clear (Black);
        _delay_ms ( 1000 );
    }

    return 0;
}
