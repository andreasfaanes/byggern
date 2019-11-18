#include "include/usercontrol.h"
#include "include/adc.h"
#include "include/ext_mem.h"
#include "include/settings.h"
#include "include/oled.h"
#include "include/can.h"
#include "include/game.h"
#include "include/timer.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <string.h>






menu_item_t main_menu, settings, play_game,  difficulty, highscore, easy, medium, hard;

menu_item_t *current_menu;
menu_item_t *current_child;
uint8_t current_child_num = 0;

menu_item_t main_menu = {
	.parent = NULL,
	.child[0] = &play_game,
	.child[1] = &settings,
	.child[2] = &highscore,
	.title = "Main menu",
	.flag = MAIN_MENU,
	.num_child = 3
};

menu_item_t recalibrate = {
	.parent = &settings,
	.title = "Recalibrate",
	.num_child = 0,
	.child[0]=NULL,
	.flag = RECALIBRATE
};

menu_item_t settings = {
	.parent = &main_menu,
	.child[0] = &difficulty,
	.child[1] = &recalibrate,
	.title = "Settings",
	.num_child = 2,
	.flag = SETTINGS
};

menu_item_t play_game = {
	.parent = &main_menu,
	.title = "Play game",
	.num_child = 0,
	.flag = PLAY_GAME
};

menu_item_t difficulty = {
	.title = "Difficulty",
	.parent = &settings,
	.child[0] = &easy,
	.child[1] = &medium,
	.child[2] = &hard,
	.num_child = 3,
	.flag = DIFFICULTY
};

menu_item_t highscore = {
	.parent = &main_menu,
	.title = "Highscore",
	.child[0] = NULL,
	.num_child = 0,
	.flag = HIGHSCORE
};

menu_item_t easy = {
	.parent = &difficulty,
	.title = "Easy",
	.child[0] = NULL,
	.num_child = 0,
	.flag = EASY
};

menu_item_t medium = {
	.parent = &difficulty,
	.title = "Medium",
	.child[0] = NULL,
	.num_child = 0,
	.flag = MEDIUM
};

menu_item_t hard = {
	.parent = &difficulty,
	.title = "Hard",
	.child[0] = NULL,
	.num_child = 0,
	.flag = HARD
};

menu_item_t select_name = {
	.parent = &play_game,
	.title = "select name",
	.num_child = 0,
	.flag = SELECT_NAME
};



void Menu_Line_Change(char* charaters, uint8_t line, uint8_t inverted){

	Oled_Clear_Line(line);
	Oled_Print(charaters,inverted);
}

void Update_Menu_Parent(void){
	Menu_Line_Change(current_menu->title,0,0);
	uint8_t i;
	for (i = 0; i < current_menu->num_child; i++)
	{
		if (i == current_child_num)
		{
			Menu_Line_Change(current_menu->child[i]->title,i+1,1);
			}else{
			Menu_Line_Change(current_menu->child[i]->title,i+1,0);
		}
	}
	Menu_Line_Change(" ", i+2, 0);
}

void Update_Menu(void){
	Oled_Reset();

	Update_Menu_Parent();

}

void Oled_Menu_Setup(void){
	current_menu = &main_menu;
	current_child = current_menu->child[0];

	Update_Menu();
}

void Change_Child(position_t direction){
	if (direction == DOWN && (current_child_num < current_menu->num_child-1))
	{
		current_child_num += 1;
	}else if (direction == UP && (current_child_num > 0))
	{
		current_child_num -= 1;
	}
	current_child = current_menu->child[current_child_num];
}

void User_Control_Init(){
		Ext_Mem_Init();
		Adc_Init();
		Oled_Init();
		//enabels buttons as inputs
		DDRB |= (1 << DDB0)|(1<<DDB1)|(1<<DDB2);

		//sets pull ups high so joystick push to work
		PORTB |= (1<<PINB2);
}


joycon_t Joycon_Get(void){
	joycon_t joycon;
	uint8_t x,y;

	x = Adc_Read(0);
	y = Adc_Read(1);

	if (x > JOYCON_MID)
	{
		joycon.x = 100*(x-JOYCON_MID)/(0xFF-JOYCON_MID);
	}else if (x < JOYCON_MID)
	{
		joycon.x = 100*(x-JOYCON_MID)/(JOYCON_MID);
	}else
	{
		joycon.x = 0;
	}

	if (y > JOYCON_MID)
	{
		joycon.y = 100*(y-JOYCON_MID)/(0xFF-JOYCON_MID);
	}else if (y < JOYCON_MID)
	{
		joycon.y = 100*(y-JOYCON_MID)/(JOYCON_MID);
	}else
	{
		joycon.y = 0;
	}

	return joycon;
}


joycon_t Joycon_Get_Direction(void){

	joycon_t joycon = Joycon_Get();

	if(joycon.x < -50){
		joycon.direction = LEFT;
		return joycon;
	}else if(joycon.x > 50){
		joycon.direction = RIGHT;
		return joycon;
	}else if(joycon.y < -50){
		joycon.direction = DOWN;
		return joycon;
	}else if(joycon.y > 50){
		joycon.direction = UP;
		return joycon;
	}else{
		joycon.direction = NEUTRAL;
		return joycon;
	}
}

slider_postion_t Slider_Get_Postion(void){

	slider_postion_t slider_postion;

	slider_postion.left_slider = Adc_Read(3);
	slider_postion.right_slider = Adc_Read(2);
	return slider_postion;
}

void Slider_Send_Pos(void){
	slider_postion_t slider_postion;
	slider_postion = Slider_Get_Postion();
	uint8_t button_left = read_button(LEFT);
	uint8_t button_right = read_button(RIGHT);
	can_data_t data;
	data.id = slider_id;
	data.length = 4;
	data.data[0] = slider_postion.left_slider;
	data.data[1] = slider_postion.right_slider;
	data.data[2] = button_left;
	data.data[3] = button_right;//shooter
	Can_Send_Msg(&data);
}

//////////////////////////////////////////////////////////////////////////
// variables for name function
char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_";
uint8_t current_letter = 0;
uint8_t current_letter_y = 0;
bool hold_joycon_alphabet = false;
//////////////////////////////////////////////////////////////////////////

uint8_t select_pos(uint8_t x,uint8_t y){
	if (x == 0 && y == 0 )
	{
		return 0;
	}
	else if (x == 0)
	{
		return y;
	}
	else if (y == 0)
	{
		return x;
	}
	else
	{
		return x*y;
	}
}



void alphabet_navigation(void){
	joycon_t joycon = Joycon_Get_Direction();
	if (!hold_joycon_alphabet)
	{
		switch(joycon.direction){
			case DOWN:
				hold_joycon_alphabet = true;
				if (current_letter+9 < 27)
				{
					current_letter += 9;
				}
				break;
			case UP:
				hold_joycon_alphabet = true;
				if (current_letter-9 >= 0)
				{
					current_letter -= 9;
				}
				break;
			case LEFT:
				hold_joycon_alphabet = true;
				if (current_letter-1 >= 0)
				{
					current_letter --;
				}
				break;
			case RIGHT:
				hold_joycon_alphabet = true;
				if (current_letter+1 < 27)
				{
					current_letter ++;
				}
				break;
			default:
			break;
		}
	}else
	{
		if (joycon.direction == NEUTRAL)
		{
			hold_joycon_alphabet = false;
		}
	}
}

void print_alphabet(void){
	Oled_Reset();
	uint8_t y = 0;
	for (uint8_t i = 0; i < 3; i++)
	{
		Oled_Clear_Line(i);
		for (uint8_t j = 0; j < 9; j++)
		{
			if (current_letter == y)
			{
				Oled_Print_Char(alphabet[y],1,j);
			}
			else
			{
				Oled_Print_Char(alphabet[y],0,j);
			}
			y++;
		}
	}
}

char select_letter_func(void){
	current_letter = 0;

	while (read_button(LEFT) == 0)
	{
		alphabet_navigation();
		print_alphabet();
		Oled_Update();
	}
	return alphabet[current_letter];
}

void create_name(char *name, uint8_t length){
	for (uint8_t i = 0; i < length; i++)
	{
			while(read_button(LEFT) )
			{
			}
		name[i] = select_letter_func();
	}
}


//////////////////////////////////////////////////////////////////////////
char *name;
bool hold_joycon = false;
//////////////////////////////////////////////////////////////////////////

void get_name(char *name_copy){
	name_copy = name; 
}
void Menu_Navigation(void){
	joycon_t joycon = Joycon_Get_Direction();
	if (!hold_joycon)
	{
		switch(joycon.direction){
			case DOWN:
				Change_Child(DOWN);
				hold_joycon = true;
				break;
			case UP:
				Change_Child(UP);
				hold_joycon = true;
				break;
			case LEFT:
				if (current_menu->parent != NULL)
				{
					current_child = current_menu;
					current_menu = current_menu->parent;
					current_child_num = 0;
					Update_Menu();
					hold_joycon = true;
				}
				break;
			case RIGHT:
				if (current_menu->child[current_child_num] != NULL)
				{
					current_menu = current_menu->child[current_child_num];
					current_child = current_menu->child[0];
					current_child_num = 0;
					Update_Menu();
					hold_joycon = true;
				}
				break;
			default:
				break;
		}
	}else
	{
		if (joycon.direction == NEUTRAL)
		{
			hold_joycon = false;
		}
	}


}

void Menu_Functionality(void){

	switch (current_menu->flag)
	{
	case MAIN_MENU:
		Update_Menu();
		break;
	case SETTINGS:
		Update_Menu();
		break;
	case PLAY_GAME:
		if (IN_MENU == Get_Game_State())
		{
			Set_Gamestate(START_GAME);
		}
		break;
	case DIFFICULTY:
		Update_Menu();
		break;
	case HIGHSCORE:
		Update_Menu();
		char one[15];
		sprintf(one,"1st: %u",Get_Highscore(0));
		char two[15];
		sprintf(two,"2nd: %u",Get_Highscore(1));
		char three[15];
		sprintf(three,"3rd: %u",Get_Highscore(2));
		char four[15];
		sprintf(four,"4th: %u",Get_Highscore(3));
		char five[15];
		sprintf(five,"5th: %u",Get_Highscore(4));
		Menu_Line_Change("HIGHSCORES:",0,0);
		Menu_Line_Change(one,2,0);
		Menu_Line_Change(two,3,0);
		Menu_Line_Change(three,4,0);
		Menu_Line_Change(four,5,0);
		Menu_Line_Change(five,6,0);
		break;
	case RECALIBRATE:
		if (!(CALIBRATE == Get_Game_State()|| CALIBRATE_WAIT  == Get_Game_State()))
		{
			Set_Gamestate(CALIBRATE);
		}
		break;
	case SELECT_NAME:
		
		break;
	default:
		Update_Menu();
		break;
	}
}

void Node_One_Init(){
	Oled_Reset();
	Menu_Line_Change("Welcome!",0,0);
	Menu_Line_Change("We are waiting",1,0);
	Menu_Line_Change("for Node 2",2,0);
	Menu_Line_Change("While waiting",3,0);
	Menu_Line_Change("view this",4,0);
	Menu_Line_Change("beautiful Lunde",5,0);
	_delay_ms(1);
	Oled_Update();
	Oled_Update();
	_delay_ms(3000);
	Oled_Reset();
	Print_Lunde();
	uint8_t data_aquired = 0;
	can_data_t msg_recv;
	can_data_t msg_tran;
	msg_tran.id = 9;
	msg_tran.data[0] = 9;
	msg_tran.length = 1;
	
	while (data_aquired == 0)
	{
		// handshake
		Can_Send_Msg(&msg_tran);
		_delay_ms(100);
		Can_Recieve_Msg(&msg_recv);
		if (msg_recv.id == 11)
		{
			
			data_aquired = 1;
		}
		_delay_ms(100);

	}
	printf("Handshake done");
	
	_delay_ms(2000);
	Oled_Reset();
}


uint8_t read_button(uint8_t button){
	if (button == LEFT)
	{
		return PINB & ( 1 << PINB1);
	}else{
		return PINB & ( 1 << PINB0);
	}
}

void Move_To_Parent(void){
	current_menu = current_menu->parent;
}