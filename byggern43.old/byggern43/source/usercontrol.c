#include "include/usercontrol.h"
#include "include/adc.h"
#include "include/ext_mem.h"
#include "include/settings.h"
#include "include/oled.h"

#include <stdio.h>
#include <avr/io.h>
#include "include/can.h"

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

void menu_line_change(char* charaters, uint8_t line, uint8_t inverted){
	
	oled_clear_line(line);
	oled_print(charaters,inverted);
}




void update_menu(void){
	oled_reset();
	menu_line_change(current_menu->title,0,0);
	uint8_t i;
	for (i = 0; i < current_menu->num_child; i++)
	{
		if (i == current_child_num)
		{
			menu_line_change(current_menu->child[i]->title,i+1,1);
		}else{
			menu_line_change(current_menu->child[i]->title,i+1,0);
		}
	}
	menu_line_change(" ", i+2, 0);
	oled_print_custom(0,i+2);
}

void oled_menu_setup(void){
	current_menu = &main_menu;
	current_child = current_menu->child[0];
	/*
	menu_line_change(current_menu->title,0,0);
	menu_line_change(current_menu->child[0]->title,1,1);
	menu_line_change(current_menu->child[1]->title,2,0);
	*/
	update_menu();
}

void change_child(position_t direction){
	if (direction == DOWN && (current_child_num < current_menu->num_child-1))
	{
		current_child_num += 1;
	}else if (direction == UP && (current_child_num > 0))
	{
		current_child_num -= 1;
	}
	current_child = current_menu->child[current_child_num];
	update_menu();
}

void user_control_init(){
		ext_mem_init();
		adc_init();
		oled_init();
		//enabels buttons as inputs
		DDRB |= (1 << DDB0)|(1<<DDB1)|(1<<DDB2);
		
		//sets pull ups high so joystick push to work
		PORTB |= (1<<PINB2);
}


joycon_t joycon_get(void){
	joycon_t joycon;
	uint8_t x,y;
	
	x = adc_read(0);
	y = adc_read(1);
	
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


joycon_t joycon_get_direction(void){
	
	joycon_t joycon = joycon_get();
	
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



slider_postion_t slider_get_postion(void){
	
	slider_postion_t slider_postion;
	
	
	slider_postion.left_slider = adc_read(3);
	slider_postion.right_slider = adc_read(2);
	//printf("%u \r",slider_postion.right_slider);
	return slider_postion;
	
}

void slider_send_pos(void){
	slider_postion_t slider_postion;
	slider_postion = slider_get_postion();
	can_data_t data;
	data.id = slider_id;
	data.length = 2;
	data.data[0] = slider_postion.left_slider;
	data.data[1] = slider_postion.right_slider;
	can_send_msg(&data);
}

void menu_navigation(void){
	joycon_t joycon = joycon_get_direction();
	switch(joycon.direction){
		case DOWN:
			change_child(DOWN);
			break;
		case UP:
			change_child(UP);
			break;
		case LEFT:
			if (current_menu->parent != NULL)
			{
				current_child = current_menu;
				current_menu = current_menu->parent;
				current_child_num = 0;
				update_menu();
			}
			break;
		case RIGHT:
			if (current_menu->child[current_child_num] != NULL)
			{
				current_menu = current_menu->child[current_child_num];
				current_child = current_menu->child[0];
				current_child_num = 0;
				update_menu();
			}
			break;	
		default:
			break;
	}
	while(joycon.direction != NEUTRAL){joycon = joycon_get_direction();};
	//Need to update joycon in loop, since joycon can only update manually
}

