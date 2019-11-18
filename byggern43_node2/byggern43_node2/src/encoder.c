/*
 * encoder.c
 *
 * Created: 01.11.2019 13:18:27
 *  Author: vebjornt
 */
#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include  "include/encoder.h"
#include "include/motor.h"
#include "include/timer.h"
#include "include/can.h"



//Variable sin order to calibrate the encoder

int16_t max_left_pos = 0;
int16_t max_right_pos = 0;




int16_t Encoder_Read_Data(void){
	int16_t result = 0;
	uint8_t low = 0;
	uint8_t high = 0;
	PORTH &= ~((1 << PINH5) | (1 << PINH3));
	_delay_us(60);
	high |= (PINK);
	PORTH |= (1 << PINH3);
	_delay_us(60);
	low |= PINK;
	PORTH |=  (1 << PINH5);
	result = (int16_t)((high << 8) | low);
	return result;
}

void Encoder_Calibrate(void){
	//calibrates encoder in order to make the PI-regulator
	TCCR3B &= ~((1 << CS02)|(1 << CS30));
	Motor_Direction(RIGHT, 100);
	_delay_ms(1000);
	Motor_Direction(LEFT, 100);
	_delay_ms(1000);
	Motor_Direction(STOP,0);
	_delay_ms(100);
	max_left_pos = Encoder_Read_Data();
	Motor_Direction(RIGHT, 100);
	_delay_ms(1000);
	Motor_Direction(STOP,0);
	_delay_ms(100);
	max_right_pos = Encoder_Read_Data();
	TCCR3B |= (1 << CS02)|(1 << CS30);
	printf("\n\n\rcalibration done, max left value: %i, max right value: %i\n\r",max_left_pos,max_right_pos);
}

int16_t Read_Max_Left_Pos(void){
	return max_left_pos;
}
int16_t Read_Max_Right_Pos(void){
	return max_right_pos;
}