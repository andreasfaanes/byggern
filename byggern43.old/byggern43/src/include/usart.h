#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdio.h>
#include "settings.h"


#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

int USART_Transmit(char data, FILE *f);

int USART_Receive(FILE* f);

int USART_init(unsigned int ubrr);

#endif