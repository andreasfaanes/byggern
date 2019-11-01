#include "usart.h"
#include "can.h"
#include "timer.h"
#include "ADC.h"
#include "motor.h"
#include "encoder.h"
#include <asf.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int main (void)
{
	USART_init(MYUBRR);
	
	can_init();

	printf("\n\n\n\n\n begin!!! \n\r");
	DDRB |= (1 << PINB5);
	timer_init();
	adc_init();
	
	timer_1_set_top(125);
	motor_init();
	sei();
	encoder_calibrate();
	
	
	while (1)
	{


	}
	
}

/*
void game_over(void){
	
}*/
uint32_t score = 0;
ISR(ADC_vect){
	uint8_t val = ADCH;
	if(val < 70){
	//game_over();
	printf("\r val =  %d  score = %d   \n\r", val, score);
	}
	else{
		score += 1;
	}
	/*if(score >= 1000)
	{
		
		score = 0;
	}*/
}

ISR(TIMER0_COMPA_vect){
	ADCSRA |= (1<<ADSC);
}

