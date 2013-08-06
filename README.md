ili9320_spi_lib
===============

ili9320 spi driver, for use with Atmel AVR ATmega 

This driver allows you to drive an ili9320 based screen via SPI. This driver is made for use with an Atmel Atmega.

Currently tested with an ATmega16a and a HY28A 320x240 LCD, but it should work with others.

How to connect:

<pre>
LCD   ->  ATmega
SCK   ->  SCK (PB7/Pin 8)
CS    ->  PB0 (Pin 1)
SDO   ->  MISO (PB6/Pin 7)
SDI   ->  MOSI (PB5/Pin 6)
RESET ->  PB1 (Pin 2)
</pre>

Also Connect GND and 3v3 for power.


The Makefile is setup for 8MHz mode without the use of an exernal resonator/crystal. 
Also I have made everything to work easily with the current version of Xcode. Note that uploading should be done with avrdude (make flash)
