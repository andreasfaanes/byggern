/*
 * settings.h
 *
 * Created: 15.11.2019 10:12:15
 *  Author: vebjornt
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_
#include "stdio.h"
#include "avr/io.h"

#define F_CPU 16000000

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#define DAC_ADDR 0b01010000
#define COMMAND 0b00000000




#endif /* SETTINGS_H_ */