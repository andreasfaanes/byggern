/*
 * motor.h
 *
 * Created: 26.10.2019 11:14:17
 *  Author: andrefaa
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "stdio.h"
#include "avr/io.h"

#define DAC_ADDR 0b01010000
#define COMMAND 0b00000000

typedef enum DIR{
	LEFT,
	RIGHT,
	STOP
	}DIR;

void motor_init(void);
void motor_update_pos(uint8_t pos);
void motor_reset(void);
void DAC_send(uint8_t speed);
void motor_Direction(uint8_t dir,uint8_t speed);

#endif /* MOTOR_H_ */