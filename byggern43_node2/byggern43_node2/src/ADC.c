/*
 * ADC.c
 *
 * Created: 25.10.2019 14:18:24
 *  Author: henridah
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "include/ADC.h"
#include "include/can.h"


uint8_t game_on = 0;

void ADC_Init(void)
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR) | (1 << ADATE); // AVCC referance and left shifted data, auto trigger
	ADCSRA |= (1 << ADEN) | (1 << ADIE); // adc enable and interupt enable
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0); // trigger on timer 0 overflow
	DIDR0 |= (1 << ADC0D);
}

void Game_Over(uint8_t absorbed_light_intensity)
{
	//When absorbed_light_intensity is under 70 the LED-Photodiode is line is broken
	if(absorbed_light_intensity < 70){
		can_data_t data;
		data.id = 16;
		game_on = 0;
		Can_Send_Msg(&data);
	}

}


void Game_Set(uint8_t set){
	if (set == 1)
	{
		game_on = 1;
	}else{
		game_on = 0;
	}
}

uint8_t Get_Game_On(void){
	return game_on;
}

ISR(ADC_vect){
	uint8_t absorbed_light_intensity = ADCH;
	//When val is under 70 the LED-Photodiode is line is broken 
	if (game_on == 1)
	{
		Game_Over(absorbed_light_intensity);	
	}

}