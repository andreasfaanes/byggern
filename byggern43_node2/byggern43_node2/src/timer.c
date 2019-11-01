/*
 * timer.c
 *
 * Created: 25.10.2019 09:35:02
 *  Author: henridah
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include <stdio.h>

void timer_init(void){	
	//Timer 0:
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	OCR0A = 127;
	TIMSK0 |= (1 << OCIE0A);
	//Configure 8 bit timer in fast PWM
	//TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Enable Fast PWM mode 3, mark first
	///TCCR0B = (1 << CS02) |(0 << CS01) | (0 << CS00); //set clock prescaler to 16MHz/256 = 62.5kHz (PWM frequency = 244.1Hz)
	//OCR0B = 128; //sets the desired pulse width (0-255)
	//DDRG |= (1 << PG5); // Set pin 4 (PG5) as output
	
	//Timer 1: clear OC1A (channel A) on compare match, use fast PWM, OCR used as TOP, prescaler to 8
	TCCR1A |= (1 << COM1A1) | (1 << WGM11) /*| (1 << WGM10)*/;
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10) | (1 << CS11);
	ICR1 = 5000;
	OCR1A = 300;
	
	//timer 2:
	TCCR3A |= (1 << WGM31);
	TCCR3B |= (1 << CS02)|(1 << CS30);
	OCR3A = 250;
	TIMSK3 |= (1 << OCIE3A);
}

void timer_1_set_top(uint16_t top_val){
	//Formula in ordr to make the dutycycle of the PWM. Done by computing the prescaler. Had to invert the input (100-top_val) in order to get the orientation right
	uint16_t x = (((255-top_val)*10*25)/(214))+(225);
	//printf("%u   \r",x);
	if (x <= 225)
	{
		x = 225;
	}else if (x >= 520)
	{
		x = 520;
	}
	
	OCR1A = x;
}

