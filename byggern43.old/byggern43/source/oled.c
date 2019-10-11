/*
 * oled.c
 *
 * Created: 20.09.2019 12:53:52
 *  Author: andrefaa
 */ 

#include "include/oled.h"

#include "include/settings.h"
#include "include/ext_mem.h"
#include "include/asciitable.h"

#include <stdio.h>
#include <avr/io.h>

void oled_command_write(uint8_t command)
{
	volatile char *oled_command = (char *) 0x1000;
	*oled_command = command;
}

void oled_data_write(uint8_t data, uint8_t column)
{
	volatile char *saved_line = (char *) 0x1c00;
	uint8_t line = saved_line[0];
	
	volatile char *oled_data= (char *)(0x1800+(0x0080*line)+column);
	//printf("%u   %04x \n\r",data, 0x1800+(0x0080*line)+column);
	*oled_data = data;
}



void oled_pos(uint8_t row){
	/*
	oled_command_write(OLED_SET_COLUMN_ADDR_RANGE);
	oled_command_write(column);
	oled_command_write(0x7f);
	
	oled_command_write(OLED_SET_PAGE_ADDR);
	oled_command_write(row);
	oled_command_write(0x07);
	*/
	
	volatile char *oled_data = (char *) 0x1c00;
	*oled_data = row;
	
	//printf("current line  %04x\n\r",*oled_data);
}

void oled_goto_line(uint8_t line){
	oled_pos(line);
}

void oled_clear_line(uint8_t line){
	oled_goto_line(line);
	
	for (uint8_t i = 0; i < 128; i++)
	{
		oled_data_write(0x00,i);
	}
	oled_goto_line(line);
}

void oled_home(void){
	oled_pos(0);
}

void oled_reset(void){
	for (uint8_t i = 0; i< 8; i++)
	{
		oled_clear_line(i);
	}
	oled_home();
}

void oled_set_brightness(uint8_t lvl){
	oled_command_write(OLED_SET_CONTRAST_CONTR);
	oled_command_write(lvl);
}

void oled_print_char(char character,uint8_t inverted, uint8_t column){
	char oled_char[8];
	ascii_to_bitmap_font8(oled_char, character);
	if (inverted == 1)
	{	
		for (uint8_t i = 0; i<8; i++)
		{
			oled_data_write(oled_char[i]^0xff,column*0x08+i);
		}
	}else
	{
		for (uint8_t i = 0; i<8; i++)
		{
			oled_data_write(oled_char[i],column*0x08+i);
		}
	}

}

void oled_print_custom(char character,uint8_t column){
	char bits[16];
	ascii_to_bitmap_custom(bits,character);
	for (uint8_t i = 0; i <16 ; i++)
	{
		oled_data_write(bits[i]^0xff,column*0x08+i);
	}
}

void print_lunde(void){
	volatile char *oled_data;
	for (uint16_t i = 0; i < 128; i++)
	{
		oled_data = (char *)(0x1800+i);
		oled_data = lunde_byte(i);
		printf("%x",lunde_byte(i));
	}	
}

void oled_init(void){
	oled_command_write(0xae); // display off
	oled_command_write(0xa1); //segment remap
	
	oled_command_write(0xda); //common pads hardware: alternative
	oled_command_write(0x12);
	
	oled_command_write(0xc8); //common
	
	oled_command_write(0x20); //Set Memory Addressing Mode
	oled_command_write(0x00); //horizontal addressing
	
	oled_command_write(0xa8); //multiplex ration mode:63
	oled_command_write(0x3f);
	
	oled_command_write(0xd5); //display
	oled_command_write(0x80);
	
	oled_command_write(0xdb); //VCOM deselect level mode
	oled_command_write(0x30);
	
	oled_command_write(0x81); //contrast control
	oled_command_write(0x50);
	
	oled_command_write(0xd9); //set pre-charge period
	oled_command_write(0x21);
	
	oled_command_write(0xad); //master configuration
	oled_command_write(0x00);
	
	oled_command_write(0xa4); //out follows RAM content
	
	oled_command_write(0xa6); //set normal display
	
	oled_command_write(0xaf); // display on
	
	oled_reset();
}

void oled_print(char* characters, uint8_t inverted){
	int char_number = 0;
	
	while((characters[char_number] != '\0') && (char_number < 16))
	{
		//printf("%i\n\r", char_number);
		oled_print_char(characters[char_number],inverted,char_number);
		char_number++;
	}
}

void oled_update(void){
	oled_command_write(OLED_SET_COLUMN_ADDR_RANGE);
	oled_command_write(0);
	oled_command_write(0x7f);
	
	oled_command_write(OLED_SET_PAGE_ADDR);
	oled_command_write(0);
	oled_command_write(0x07);
	
	volatile char *sram_read = (char *) (0x1800);
	volatile char *oled_write = (char *) 0x1200;
	
	for (uint16_t i = 0; i < 0x400; i++)
	{
		*oled_write = sram_read[i];
	}
}