/*
 * spi.c
 *
 * Created: 04.10.2019 09:26:25
 *  Author: henridah
 */

#include <avr/io.h>
#include <stdio.h>
#include "include/spi.h"

void Spi_Init(void){
	DDRB |= (1 << PINB4)|(1 << PINB5)|(1 << PINB7); // setting ss, mosi and sck as output
	DDRB &= ~(1 << PINB6);	// setting miso as input
	SPCR |= (1<<SPE)|(1<<MSTR)|(1 << SPR0); // enable SPI and selectiong master mode
}

void Spi_Write(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

uint8_t Spi_Read(void){
	SPDR = 0xaa;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void Slave_Enable(void){
	PORTB &= ~(1 << PINB4);
}

void Slave_Deselect(void){
	PORTB |= (1 << PINB4);
}