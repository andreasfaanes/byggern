#include <stdio.h>

typedef enum {NEUTRAL, LEFT, RIGHT, UP, DOWN} position_t;
	
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
};

void user_control_init();

joycon_t joycon_get(void);
joycon_t joycon_get_direction(void);
slider_postion_t slider_get_postion(void);
void oled_menu_setup(void);
void menu_line_change(char* charaters, uint8_t line,uint8_t inverted);
void menu_navigation(void);
void slider_send_pos(void);