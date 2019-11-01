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
#include "motor.h"
#include "timer.h"
#include "can.h"

int16_t measured = 0; 
int16_t pos = 0;
int16_t max_left_pos = 0;
int16_t max_right_pos = 0;
int16_t slider_pos = 125;
double integral = 0;
double dt = 0.010; //temporary value, not sure of runtime of timer 3
double Kp = 0.04;
double Ki = 0.02;


int16_t encoder_read_data(void){
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

void encoder_calibrate(void){
	TCCR3B &= ~((1 << CS02)|(1 << CS30));
	motor_Direction(RIGHT, 90);
	_delay_ms(1000);
	motor_Direction(LEFT, 90);
	_delay_ms(1000);
	motor_Direction(STOP,90);
	_delay_ms(100);
	max_left_pos = encoder_read_data();
	motor_Direction(RIGHT, 100);
	_delay_ms(1000);
	motor_Direction(STOP,0);
	_delay_ms(100);
	max_right_pos = encoder_read_data();
	TCCR3B |= (1 << CS02)|(1 << CS30);
	printf("\n\n\rcalibration done, max left value: %i, max right value: %i\n\r",max_left_pos,max_right_pos);
}

void print_pos(void){
	printf("%i\n\r",pos);
}


can_data_t data;
ISR(TIMER3_COMPA_vect){
	can_recieve_msg(&data);
	if (data.id != 0)
	{
		timer_1_set_top(data.data[0]);
		slider_pos = 255 - data.data[1];
	}
	
	pos = encoder_read_data();
	int16_t difference = abs(max_left_pos - max_right_pos);
	int16_t scaled_slider_ref = (difference/255)*slider_pos;
	int16_t error = 0;
	if (abs(scaled_slider_ref - pos)<50){
		error = 0;
		integral = 0;
	}else{
		error = scaled_slider_ref - pos;
		integral = integral + error*dt;
	}
	
	
	
	int16_t power_signed = Kp*error + Ki*integral;
	uint16_t power= 0;
	
	if (power_signed > 0){
		if(power_signed < 150){
			power = power_signed;
		}else{
			power = 150;
		}
		motor_Direction(LEFT,power);		
	}else{
		if(power_signed > -150){
			power = -power_signed;
		}else{
			power = 150;
		}
		motor_Direction(RIGHT,power);
	} 
	
	//printf("%i  %u\n\r",error, power); 
}
