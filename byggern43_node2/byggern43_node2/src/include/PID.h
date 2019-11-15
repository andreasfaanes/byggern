/*
 * PID.h
 *
 * Created: 15.11.2019 10:10:51
 *  Author: vebjornt
 */ 


#ifndef PID_H_
#define PID_H_

#include "stdio.h"
#include "avr/io.h"


int16_t Calculate_PID_Power(int16_t slider_pos, int16_t pos);
void Set_Power_and_Direction(int16_t power_signed);

void Set_PID_Diffiuculty(void);


#endif /* PID_H_ */