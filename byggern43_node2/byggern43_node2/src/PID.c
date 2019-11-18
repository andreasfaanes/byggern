/*
 * CFile1.c
 *
 * Created: 15.11.2019 10:08:04
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
#include "include/PID.h"


double integral = 0;
double dt = 0.010; //Works fine, but not sure if its right
double Kp = 0.04;
double Ki = 0.02;

uint8_t dummy_difficulty;



int16_t Calculate_PID_Power(int16_t slider_pos, int16_t pos){
	

	//scales the regulator according to the resolution provided by the encoder
	int16_t difference = abs(Read_Max_Left_Pos() - Read_Max_Right_Pos());
	int16_t scaled_slider_ref = (difference/255)*slider_pos;
	int16_t error = 0;
	
	//calculates error and checks if the regulator is inside the error margin
	if (abs(scaled_slider_ref - pos)<50){
		error = 0;
		integral = 0;
		}else{
		error = scaled_slider_ref - pos;
		integral = integral + error*dt;
	}
	
	
	
	int16_t power_signed = Kp*error + Ki*integral;
	return power_signed;
}



void Set_Power_and_Direction(int16_t power_signed){
	
	uint16_t power= 0;
	//decides power and direction, sets saturation for power
	if (power_signed > 0){
		if(power_signed < 150){
			power = power_signed;
			}else{
			power = 150;
		}
		Motor_Direction(LEFT,power);
		}else{
		if(power_signed > -150){
			power = -power_signed;
			}else{
			power = 150;
		}
		Motor_Direction(RIGHT,power);
	}
}


void Set_PID_Diffiuculty(void){
	//Can_Recieve_Msg(&data);
	//Easy
	if(dummy_difficulty == 0){
		Kp = 0.04;
		Ki = 0.02;
	//Medium
	}else if(dummy_difficulty == 1){
		Kp = 0.08;
		Ki = 0.02;
	//Hard
	}else if(dummy_difficulty==2){
		Kp = 0.005;
		Ki = 0.005;
	}
}