/*
 * solenoid.h
 *
 * Created: 15.11.2019 13:24:04
 *  Author: vebjornt
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_
#include <stdio.h>

void Init_Solenoid(void);
void Trigger_Solenoid(void);
void Disable_Solenoid(void);
uint8_t Get_Shot_Recently(void);
void Set_Shot_Recently(uint8_t state);



#endif /* SOLENOID_H_ */