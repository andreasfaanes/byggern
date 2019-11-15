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
			if (Get_Game_State() == CALIBRATE)
			{
				Move_To_Parent();
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
	printf("begin");
	DDRD |=(1<<PIND5);
	Timer_3_off();
    Timer_Init();
	Timer_3_off();
	User_Control_Init();
	printf("here?");
    Can_Init();
	Node_One_Init();
	
	
	Oled_Menu_Setup();
	//Timer_3_on();
	Timer_3_off();
	sei();
    while (1)
    {
		Game_Machine();
		Oled_Update();
		Menu_Functionality();
		can_check_message();
		_delay_ms(100);
    }
}



ISR(TIMER0_OVF_vect){

}

ISR(TIMER3_COMPA_vect){
	//printf("herno");

	
}