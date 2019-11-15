#include "include/usercontrol.h"
#include "include/adc.h"
#include "include/ext_mem.h"
#include "include/settings.h"
#include "include/oled.h"
#include "include/can.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>



typedef enum {
	MAIN_MENU,
	SETTINGS,
	PLAY_GAME,
	DIFFICULTY
}menu_flag_t;


menu_item_t main_menu, settings, play_game,  difficulty;

menu_item_t *current_menu;
menu_item_t *current_child;
uint8_t current_child_num = 0;

menu_item_t main_menu = {
	.parent = NULL,
	.child[0] = &play_game,
	.child[1] = &settings,
	.title = "Main menu",
	.flag = MAIN_MENU,
	.num_child = 2
};

menu_item_t settings = {
	.parent = &main_menu,
	.child[0] = &difficulty,
	.title = "Settings",
	.num_child = 1,
	.flag = SETTINGS
};

menu_item_t play_game = {
	.parent = &main_menu,
	.title = "Play game",
	.num_child = 0,
	.flag = PLAY_GAME
};

menu_item_t difficulty = {
	.title = "difficulty",
	.parent = &settings,
	.child[0] = NULL,
	.num_child = 0,
	.flag = DIFFICULTY
};


void Menu_Line_Change(char* charaters, uint8_t line, uint8_t inverted){

	Oled_Clear_Line(line);
	Oled_Print(charaters,inverted);
}

void Update_Menu(void){
	Oled_Reset();
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
	Oled_Print_Custom(0,i+2);
}

void Oled_Menu_Setup(void){
	current_menu = &main_menu;
	current_child = current_menu->child[0];
	/*
	menu_line_change(current_menu->title,0,0);
	menu_line_change(current_menu->child[0]->title,1,1);
	menu_line_change(current_menu->child[1]->title,2,0);
	*/
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
	Update_Menu();
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
	data.data[3] = button_right;
	Can_Send_Msg(&data);
}

bool hold_joycon = false;
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
		/* code */
		break;
	case SETTINGS:
		break;
	case PLAY_GAME:
		Slider_Send_Pos();
		break;
	case DIFFICULTY:
		break;
	default:
		break;
	}
}


uint8_t read_button(uint8_t button){
	if (button == LEFT)
	{
		return PINB & ( 1 << PINB1);
	}else{
		return PINB & ( 1 << PINB0);
	}
}