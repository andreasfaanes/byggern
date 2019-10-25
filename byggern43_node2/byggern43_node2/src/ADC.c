/*
 * ADC.c
 *
 * Created: 25.10.2019 14:18:24
 *  Author: henridah
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

void adc_init(void)
{
	printf("he");
	ADMUX |= (1 << REFS0) | (1 << ADLAR) | (1 << ADATE); // AVCC referance and left shifted data, auto trigger
	ADCSRA |= (1 << ADEN) | (1 << ADIE); // adc enable and interupt enable
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0); // trrigger on timer 0 overflow
	DIDR0 |= (1 << ADC0D);
	printf("he2");
}

