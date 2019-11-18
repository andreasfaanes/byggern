/*
 * byggern_node_1.c
 *
 * Created: 15.11.2019 08.30.38
 * Author : hnd00
 */
#include "sources/include/settings.h"

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "sources/include/usercontrol.h"
#include "sources/include/oled.h"
#include "sources/include/usart.h"
#include "sources/include/timer.h"
#include "sources/include/can.h"
#include "sources/include/game.h"

void can_check_message(void){
	can_data_t recived_data;
	recived_data.id = 0;
	Can_Recieve_Msg(&recived_data);
	
	switch(recived_data.id){
		case (10):
		
			break;
		case (11):
			break;
		case (12):
			break;
		case (13):
			break;
		case (14):
			break;
		case (15):
			break;
		case (16):// game over
			if (Get_Game_State() == IN_GAME)
			{
				Set_Gamestate(END_GAME);
			}
			break;
		case (17): // wait for calibrate to finish
			if (Get_Game_State() == CALIBRATE_WAIT)
			{					
				Set_Gamestate(IN_MENU);				
			}
			break;
		case (18):
			break;
		case (19):
			break;
		default:
			break;
		
	}
}

int main(void)
{
	cli();
	USART_Init(MYUBRR);
	printf("\n\n\rstart of program\n\r");
	DDRD |=(1<<PIND5);
	
    Timer_Init();
	Timer_3_off();
	
	User_Control_Init();
    Can_Init();
	
	printf("\n\rwaiting on node 1\n\r");
	
	Node_One_Init();

	Oled_Menu_Setup();
	
	sei();
	Timer_3_on();
	printf("\n\rinit done\n\r");

    while (1)
    {
		can_check_message();
		Oled_Update();
		Game_Machine();
		Menu_Functionality();
		//_delay_ms(100);
    }
}



ISR(TIMER0_OVF_vect){
	printf("timer 0 test ");
}

uint8_t program_counter = 0;

ISR(TIMER3_COMPA_vect){
	//printf("timer 3 test ");
	//
	//Oled_Update();
	//if (program_counter%100 == 0)
	//{
	//	Game_Machine();
	//}
	//if (program_counter == 250)
	//{
	//	Menu_Functionality();
	//	program_counter = 0;
	//}

}