/*
 * PWM.c
 *
 * Created: 18.10.2019 13:10:19
 *  Author: henridah
 */

#include <avr/io.h>
#include "include/timer.h"
#include "include/settings.h"
#include <util/delay.h>

void Timer_Init(void){

	//Timer 0: Interrupt on timer overflow, prescaler set to 8
	TIMSK=(1<<TOIE0);
	TCNT0=0x00; // start value for counter
	//TCCR0 = (1<<CS01 )|(1<<CS00);


	//Timer 1: Toggle OC1A (channel A) on compare match, use fast PWM, OCR1A used as TOP, prescaler to 8
	TCCR1A |= (1 << COM1A0) | (1 << WGM11) | (1 << WGM10);
	TCCR1A &= ~(1 << COM1A1);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);


	//Timer 3: Normal port operation, CTC mode, OCR3A as TOP
	TCCR3A &= ~(1 << COM3A1) & ~(1 << COM3A0) & ~(1 << WGM31) & ~(1 << WGM30);
	TCCR3B &= ~(1 << WGM33);
	TCCR3B |= (1 << WGM32);
	OCR3A = 500;
	ETIMSK |= (1 << OCIE3A);//Set the compare match interrupt enable
}

void Timer_1_on(void){
	//set timer 1 prescale to 8
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10);
}

void Timer_1_off(void){
	//set timer 1 prescale to no clock
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10) & ~(1 << CS11);
}

void Timer_1_Set_Top(uint16_t top_val){
	if (top_val == 0)
	{
		Timer_1_off();
	}
	else{
		Timer_1_off();
		_delay_ms(5);
		OCR1A = top_val;
		Timer_1_on();
	}
}

void Timer_3_on(void){
	//set timer 3 prescale to 256
	TCCR3B |= (1 << CS32);
	TCCR3B &= ~(1 << CS31) & ~(1 << CS30);
}

void Timer_3_off(void){
	//set timer 3 prescale to no clock
	TCCR3B &= ~(1 << CS32) & ~(1 << CS31) & ~(1 << CS30);
}

void Timer_3_Set_Top(uint16_t top_val){
	if (top_val == 0)
	{
		Timer_3_off();
	}
	else{
		Timer_3_off();
		_delay_ms(5);
		OCR1A = top_val;
		Timer_3_on();
	}
}
