/*
 * motor.c
 *
 * Created: 26.10.2019 11:14:05
 *  Author: andrefaa
 */ 
#define F_CPU 16000000
#include "TWI_Master.h"
#include "motor.h"
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

void motor_init(void){
	DDRH |= (1 << DDH1)| (1 << DDH4)| (1 << DDH5) |(1 << DDH6) | (1 << DDH3) ;
	PORTH |= (1 << PINH4);
	DDRD |= ((1 << DDD0) | (1 << DDD1));
	motor_reset();
	DDRK = 0x00;
	TWI_Master_Initialise();
}

void motor_update_pos(uint8_t pos){
	
	uint8_t message[3];
	message[0] = DAC_ADDR;
	message[1] = COMMAND;
	message[2] = pos;
	uint8_t message_length = 3;
	uint8_t a = TWI_Start_Transceiver_With_Data(message, message_length);
	//printf("err = 0x%x\n\r",a);
}



void update_Motor_Direction_and_Speed(uint16_t sliderpos)
{	
	
	
}
void motor_reset(void){
	PORTH &= ~(1 << PINH6);
	_delay_us(200);
	PORTH |= (1 << PINH6);
}
void motor_Direction(uint8_t dir,uint8_t speed)
{
	if(dir == LEFT){
		PORTH &= ~(1 << PINH1);
		DAC_send(speed);
	}
	else if(dir == RIGHT){
		PORTH |= (1 << PINH1);
		DAC_send(speed);
	}else{
		DAC_send(0);
	}
}


void DAC_send(uint8_t data) {
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