
#include "usart.h"
#include <stdio.h>
#include <avr/io.h>

FILE* usart;


// USART driver

// USART transmit
int USART_Transmit(char data, FILE *f){
	/*Wait for empty transmit buffer*/
	while(!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data*/
	UDR0 = data;
	
	return 0;
}

int USART_Receive(FILE* f){
	/* Wait for data to be received*/
	while(!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer*/
	return UDR0;
}
// USART init
int USART_init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	/*Enable receiver and transmitter and receive interrupt*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<< RXCIE0);
	/*Set frame format: 8data, 2 stop bit*/
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	
	usart = fdevopen(&USART_Transmit, &USART_Receive);
	
	return 0;
}
/*
ISR(USART0_RXC_vect){
			if (USART_Receive() == 'a')
			{
				
				USART_Transmit('k');
				USART_Transmit('k');
			}
}
*/