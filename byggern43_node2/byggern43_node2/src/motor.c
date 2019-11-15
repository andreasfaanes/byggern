/*
 * motor.c
 *
 * Created: 26.10.2019 11:14:05
 *  Author: andrefaa
 */ 
#include "include/settings.h"
#include "include/TWI_Master.h"
#include "include/motor.h"
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

void Motor_Init(void){
	DDRH |= (1 << DDH1)| (1 << DDH4)| (1 << DDH5) |(1 << DDH6) | (1 << DDH3) ;
	PORTH |= (1 << PINH4);
	DDRD |= ((1 << DDD0) | (1 << DDD1));
	Motor_Reset();
	DDRK = 0x00;
	TWI_Master_Initialise();
}

void Motor_Update_Pos(uint8_t pos){
	
	uint8_t message[3];
	message[0] = DAC_ADDR;
	message[1] = COMMAND;
	message[2] = pos;
	uint8_t message_length = 3;
	TWI_Start_Transceiver_With_Data(message, message_length);
	
}



void Motor_Reset(void){
	PORTH &= ~(1 << PINH6);
	_delay_us(200);
	PORTH |= (1 << PINH6);
}

void Motor_Direction(uint8_t dir,uint8_t speed)
{
	if(dir == LEFT){
		PORTH &= ~(1 << PINH1);
		DAC_Send(speed);
	}
	else if(dir == RIGHT){
		PORTH |= (1 << PINH1);
		DAC_Send(speed);
	}else{
		DAC_Send(0);
	}
}


void DAC_Send(uint8_t data) {
	uint8_t address = 0b01011110;
	uint8_t command = 0b0;
	
	uint8_t message[3];
	message[0] = address;
	message[1] = command;
	message[2] = data;
	
	uint8_t a = TWI_Start_Transceiver_With_Data(message, 3);
	_delay_us(50);
	if (a != 0xf8)
	{
		printf("err = 0x%x\n\r",a);
	}
}