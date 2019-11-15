#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdio.h>
#include "settings.h"

/**
 *  Init of the Usart module
 *
 *  ubbr, the baud rate for the uart
 **/
int USART_Init(unsigned int ubrr);

int USART_Transmit(char data, FILE *f);

int USART_Receive(FILE* f);



#endif