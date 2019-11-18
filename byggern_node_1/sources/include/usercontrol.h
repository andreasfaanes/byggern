#include <stdio.h>

typedef enum {
	NEUTRAL, 
	LEFT, 
	RIGHT, 
	UP, 
	DOWN
}position_t;

typedef enum {
	MAIN_MENU,
	SETTINGS,
	PLAY_GAME,
	HIGHSCORE,
	DIFFICULTY,
	EASY,
	MEDIUM,
	HARD,
	RECALIBRATE,
	SELECT_NAME
}menu_flag_t;

typedef struct
{
	int8_t x;
	int8_t y;
	position_t direction;
}joycon_t;

typedef struct
{
	uint8_t left_slider;
	uint8_t right_slider;
}slider_postion_t;

typedef struct menu_item menu_item_t;

typedef struct menu_item
{
	char * title;
	menu_item_t *child[6];
	menu_item_t *parent;
	uint8_t num_child;
	uint8_t flag;
}menu_item;




/**
 * 	initalize the user controll
 *
 * 	also initalize the external memory, ADC and OLED. *
 **/
void User_Control_Init();

joycon_t Joycon_Get(void);
joycon_t Joycon_Get_Direction(void);
slider_postion_t Slider_Get_Postion(void);
void Oled_Menu_Setup(void);
void Menu_Line_Change(char* charaters, uint8_t line,uint8_t inverted);
void Menu_Navigation(void);
void Slider_Send_Pos(void);
void Menu_Functionality(void);
uint8_t read_button(uint8_t button);
void Update_Menu_Parent(void);
void Node_One_Init();
void Update_Menu(void);
void create_name(char *name, uint8_t length);
void Move_To_Parent(void);