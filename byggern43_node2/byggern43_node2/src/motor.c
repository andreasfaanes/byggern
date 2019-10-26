/*
 * motor.c
 *
 * Created: 26.10.2019 11:14:05
 *  Author: andrefaa
 */ 

#include "TWI_Master.h"
#include "motor.h"
#include <stdio.h>
#include <avr/io.h>

void motor_init(void){
	DDRH |= (1 << DDH1)| (1 << DDH4)| (1 << DDH5) |(1 << DDH6) | (1 << DDH3) ;
	PORTH |= (1 << PINH4);
	TWI_Master_Initialise();
}

void motor_update_pos(uint8_t pos){
	
	uint8_t message[3];
	message[0] = DAC_ADDR;
	message[1] = COMMAND;
	message[2] = pos;
	uint8_t message_length = 3;
	uint8_t a = TWI_Start_Transceiver_With_Data(message, message_length);
	printf("err = 0x%x\n\r",a);
}



void motorDirection(uint8_t dir)
{
	if(dir){
		PORTH &= ~(1 << PINH1);
	}
	else{
		PORTH |= (1 << PINH1);
	}
}