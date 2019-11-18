/*
 * game.c
 *
 * Created: 15.11.2019 20:32:34
 *  Author: vebjornt
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "include/usercontrol.h"
#include "include/can.h"
#include "include/settings.h"
#include "include/game.h"
#include "include/oled.h"
#include <util/delay.h>

//////////////////////////////////////////////////////////////////////////
// global game variables

uint8_t game_state = INITIAL;
uint16_t score = 0;
uint8_t counter = 0;
uint16_t list_highscore[5];
can_data_t start_game;

//////////////////////////////////////////////////////////////////////////


// main state machine for the game

void Game_Machine(void){
	can_data_t calibrate_msg;
	uint16_t temp = 0;
	uint16_t temp2 = 0;
	uint8_t check = 0;
	start_game.id = 2;
	switch(game_state){
		case INITIAL:

			for(uint8_t i = 0; i<5; i++){
				list_highscore[i] = 0;	
			}

			game_state = IN_MENU;
			
			break;
			
		case START_GAME: // init the single game
			
			score = 0;
			Menu_Line_Change("GAME STARTS IN:",0,0);
			char countdown[2];
			for(uint8_t i = 0; i <3; i++){
				sprintf(countdown,"%u",3-i);
				Menu_Line_Change(countdown,3,0);
				Oled_Update();
				_delay_ms(1000);
			}
			Oled_Reset(); 
			Can_Send_Msg(&start_game);
			game_state = IN_GAME;
			break;
			
		case IN_GAME:
			Slider_Send_Pos();
			char your_score[15];
			char highest_score[15];
			sprintf(your_score,"YOUR SCORE: %u",score);
			Menu_Line_Change(your_score,1,0);
			sprintf(highest_score,"%u",list_highscore[0]);
			Menu_Line_Change("HIGHEST SCORE:",4,0);
			Menu_Line_Change(highest_score,5,0);
			
			counter ++;
			if(counter > 20){
				score ++;
				counter = 0;
			}
			break;
			
		case IN_MENU:
			Menu_Navigation();
			Update_Menu();
			break;
			
		case (END_GAME): 

			for (uint8_t i = 0; i<5; i++)
			{
				if(check == 1)
				{	
					temp2 = list_highscore[i];
					list_highscore[i] = temp;
					temp = temp2;
					
				}
				else if (score > list_highscore[i] && check == 0)
				{	
					temp = list_highscore[i];
					list_highscore[i] = score;
					check = 1;
				}
			}
			score = 0;
			Move_To_Parent();
			game_state = IN_MENU;
			break;
			
		case CALIBRATE:
			
			calibrate_msg.id = 8;
			calibrate_msg.data[0] = 8;
			calibrate_msg.length = 1;
			Can_Send_Msg(&calibrate_msg);
			Set_Gamestate(CALIBRATE_WAIT);
			break;
			
		case CALIBRATE_WAIT:
			Move_To_Parent();
			Set_Gamestate(IN_MENU);
			break; 
	}
}

void Set_Gamestate(game_state_t state){
	game_state = state;
}

game_state_t Get_Game_State(void){
	return game_state;
}

uint16_t Get_Highscore(uint8_t number){
	return list_highscore[number];
}