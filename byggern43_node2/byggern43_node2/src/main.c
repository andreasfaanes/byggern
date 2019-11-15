#include "include/settings.h"
#include "include/usart.h"
#include "include/can.h"
#include "include/timer.h"
#include "include/ADC.h"
#include "include/motor.h"
#include "include/encoder.h"
#include "include/solenoid.h"
#include <asf.h> //test if can be commented out
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void Node_Two_Init(void){
	uint8_t data_aquired = 0;
	can_data_t msg_recv;
	can_data_t msg_tran;
	msg_tran.id = 11;
	msg_tran.data[0] = 11;
	msg_tran.length = 1;
	
	//handshake
	while (!data_aquired)
	{
		Can_Recieve_Msg(&msg_recv);
		if (msg_recv.id == 9)
		{
			data_aquired = 1;
			Can_Send_Msg(&msg_tran);
		}
	}
	
	Encoder_Calibrate();
	msg_tran.id = 10;
	msg_tran.data[0] = 10;
	msg_tran.length = 1;
	Can_Send_Msg(&msg_tran);
	printf("sendt");
}

int main (void)
{
	USART_Init(MYUBRR);
	
	Can_Init();

	printf("\n\n\n\n\n begin!!! \n\r");
	DDRB |= (1 << PINB5);
	Timer_Init();
	ADC_Init();
	
	Timer_1_Set_Top(300);
	Motor_Init();
	Init_Solenoid();
	sei();
	Node_Two_Init();
	
	can_data_t data;
	
	while (1)
	{
		data.id = 0;
		Can_Recieve_Msg(&data);
		switch(data.id){
			case(1):
				//Sets PWM for left slider in order to control svervo
				Timer_1_Set_Top(data.data[0]);
				//Calculates slater reference for PID
				Set_Slide_Pos(255 - data.data[1]);
				//checks if a shot is fired from the solenoid.
				set_shoot_solenoid_status (data.data[3]);
				break;
			case(8):
				timer_3_off();
				can_data_t calibrate_recv;
				calibrate_recv.id = 18;
				can_data_t calibrate_done;
				calibrate_done.id = 17;
				Can_Send_Msg(&calibrate_recv);
				printf("calibrateing\n\r");
				Encoder_Calibrate();
				Can_Send_Msg(&calibrate_done);
				printf("recalibrate done \n\r");
				timer_3_on();
				break;
			default:
				break;
		}
		if(Get_Shoot_Solenoid_Status()==1 && Get_Shot_Recently() == 0){
			Trigger_Solenoid();
			Disable_Solenoid();
		}
	}
	
}

ISR(TIMER0_COMPA_vect){
	ADCSRA |= (1<<ADSC);
}

