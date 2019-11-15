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

typedef enum DIR{
	LEFT,
	RIGHT,
	STOP
	}DIR;

void Motor_Init(void);
void Motor_Update_Pos(uint8_t pos);
void Motor_Reset(void);
void DAC_Send(uint8_t speed);
void Motor_Direction(uint8_t dir,uint8_t speed);

#endif /* MOTOR_H_ */