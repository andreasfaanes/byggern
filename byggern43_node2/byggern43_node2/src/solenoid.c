/*
 * CFile1.c
 *
 * Created: 15.11.2019 13:23:05
 *  Author: vebjornt
 */ 
#include "include/settings.h"
#include <avr/io.h>
#include <util/delay.h>
#include "include/solenoid.h"

uint8_t shot_recently = 0;

void Init_Solenoid(void){
	DDRE |=(1<<DDE4);
	PORTE |=(1<<PINE4);
}

void Trigger_Solenoid(void){
	PORTE &= ~(1 << PINE4);

}

void Disable_Solenoid(void){
	_delay_ms(100);
	PORTE |= (1 << PINE4);
	_delay_ms(10);
	shot_recently = 1;
}

uint8_t Get_Shot_Recently(void){
	return shot_recently;
}

void Set_Shot_Recently(uint8_t state){
	shot_recently = state;
}