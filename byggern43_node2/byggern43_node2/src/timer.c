/*
 * timer.c
 *
 * Created: 25.10.2019 09:35:02
 *  Author: henridah
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "include/timer.h"
#include <stdio.h>

void Timer_Init(void){	
	//Timer 0:
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	OCR0A = 127;
	TIMSK0 |= (1 << OCIE0A);
	
	
	//Timer 1: clear OC1A (channel A) on compare match, use fast PWM, OCR used as TOP, prescaler to 8
	TCCR1A |= (1 << COM1A1) | (1 << WGM11) /*| (1 << WGM10)*/;
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10) | (1 << CS11);
	ICR1 = 5000;
	OCR1A = 300;
	
	//timer 3:
	TCCR3A |= (1 << WGM31);
	TCCR3B |= (1 << CS32)|(1 << CS30);
	OCR3A = 2500;
	TIMSK3 |= (1 << OCIE3A);
}

void Timer_1_Set_Top(uint16_t top_val){
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

void timer_3_off(void){
	TCCR3B &= ~((1 << CS32)|(1 << CS30));
}

void timer_3_on(void){
	TCCR3B |= (1 << CS32)|(1 << CS30);
}
