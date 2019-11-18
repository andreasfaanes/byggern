/*
 * game.h
 *
 * Created: 15.11.2019 20:32:46
 *  Author: vebjornt
 */ 


#ifndef GAME_H_
#define GAME_H_

typedef enum game_state_t{
	INITIAL = 0,
	START_GAME,
	IN_GAME,
	END_GAME,
	IN_MENU,
	CALIBRATE,
	CALIBRATE_WAIT,
	SAVE_SCORE	
}game_state_t;

typedef struct highscore_t{
	char *name;
	uint16_t score;
}highscore_t;

void Game_Machine(void);
game_state_t Get_Game_State(void);
void Set_Gamestate(game_state_t state);
uint16_t Get_Highscore(uint8_t number);
void Get_Highscore_Name(char * name, uint8_t number);
#endif /* GAME_H_ */