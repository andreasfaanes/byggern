#include "usart.h"
#include "can.h"
#include <asf.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int main (void)
{
	USART_init(MYUBRR);
	sei();
	can_init();
	can_data_t data;
	printf("\n\n\n\n\n begin!!! \n\r");
	while (1)
	{
		can_recieve_msg(&data);
		if (data.id != 0)
		{
			printf("\n\rID = %u, leftslider: %u, rightslider: %u \n\r",data.id, data.data[0], data.data[1]);
		}
	}
}
