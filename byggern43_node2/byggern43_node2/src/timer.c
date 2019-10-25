/*
 * timer.c
 *
 * Created: 25.10.2019 09:35:02
 *  Author: henridah
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

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
}

void timer_1_set_top(uint16_t top_val){
	uint16_t x = ((top_val*250)/84)+(225);
	if (x <= 230)
	{
		x = 230;
	}else if (x >= 520)
	{
		x = 520;
	}
	
	OCR1A = x;
}

