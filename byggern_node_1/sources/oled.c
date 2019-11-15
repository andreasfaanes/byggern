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

void Oled_Command_Write(uint8_t command)
{
	// send command to oled
	volatile char *oled_command = (char *) 0x1000;
	*oled_command = command;
}

void Oled_Data_Write(uint8_t data, uint8_t column)
{
	volatile char *saved_line = (char *) 0x1c00;
	uint8_t line = saved_line[0];

	volatile char *oled_data= (char *)(0x1800+(0x0080*line)+column);
	*oled_data = data;
}



void Oled_Pos(uint8_t row){
	volatile char *oled_data = (char *) 0x1c00;
	*oled_data = row;
}

void Oled_Goto_Line(uint8_t line){
	Oled_Pos(line);
}

void Oled_Clear_Line(uint8_t line){
	Oled_Goto_Line(line);

	for (uint8_t i = 0; i < 128; i++)
	{
		Oled_Data_Write(0x00,i);
	}
	Oled_Goto_Line(line);
}

void Oled_Home(void){
	Oled_Pos(0);
}

void Oled_Reset(void){
	for (uint8_t i = 0; i< 8; i++)
	{
		Oled_Clear_Line(i);
	}
	Oled_Home();
}

void Oled_Set_Brightness(uint8_t lvl){
	Oled_Command_Write(OLED_SET_CONTRAST_CONTR);
	Oled_Command_Write(lvl);
}

void Oled_Print_Char(char character,uint8_t inverted, uint8_t column){
	char oled_char[8];
	Ascii_to_Bitmap_Font8(oled_char, character);
	if (inverted == 1)
	{
		for (uint8_t i = 0; i<8; i++)
		{
			Oled_Data_Write(oled_char[i]^0xff,column*0x08+i);
		}
	}else
	{
		for (uint8_t i = 0; i<8; i++)
		{
			Oled_Data_Write(oled_char[i],column*0x08+i);
		}
	}

}

void Oled_Print_Custom(char character,uint8_t column){
	char bits[16];
	Ascii_to_Bitmap_Custom(bits,character);
	for (uint8_t i = 0; i <16 ; i++)
	{
		Oled_Data_Write(bits[i]^0xff,column*0x08+i);
	}
}

void Print_Lunde(void){
	volatile char *oled_write = (char *) 0x1200;
	Oled_Command_Write(OLED_SET_COLUMN_ADDR_RANGE);
	Oled_Command_Write(0);
	Oled_Command_Write(0x7f);
	Oled_Command_Write(OLED_SET_PAGE_ADDR);
	Oled_Command_Write(0);
	Oled_Command_Write(0x07);
	for (uint16_t i = 0; i < 0x400; i++)
	{
		*oled_write = Lunde_Byte(i);
	}
}

void Oled_Init(void){
	Oled_Command_Write(0xae); // display off
	Oled_Command_Write(0xa1); //segment remap

	Oled_Command_Write(0xda); //common pads hardware: alternative
	Oled_Command_Write(0x12);

	Oled_Command_Write(0xc8); //common

	Oled_Command_Write(0x20); //Set Memory Addressing Mode
	Oled_Command_Write(0x00); //horizontal addressing

	Oled_Command_Write(0xa8); //multiplex ration mode:63
	Oled_Command_Write(0x3f);

	Oled_Command_Write(0xd5); //display
	Oled_Command_Write(0x80);

	Oled_Command_Write(0xdb); //VCOM deselect level mode
	Oled_Command_Write(0x30);

	Oled_Command_Write(0x81); //contrast control
	Oled_Command_Write(0x50);

	Oled_Command_Write(0xd9); //set pre-charge period
	Oled_Command_Write(0x21);

	Oled_Command_Write(0xad); //master configuration
	Oled_Command_Write(0x00);

	Oled_Command_Write(0xa4); //out follows RAM content

	Oled_Command_Write(0xa6); //set normal display

	Oled_Command_Write(0xaf); // display on

	Oled_Reset();
}

void Oled_Print(char* characters, uint8_t inverted){
	int char_number = 0;

	while((characters[char_number] != '\0') && (char_number < 16))
	{
		Oled_Print_Char(characters[char_number],inverted,char_number);
		char_number++;
	}
}

void Oled_Update(void){
	// updated oled from Sram
	Oled_Command_Write(OLED_SET_COLUMN_ADDR_RANGE);
	Oled_Command_Write(0);
	Oled_Command_Write(0x7f);

	Oled_Command_Write(OLED_SET_PAGE_ADDR);
	Oled_Command_Write(0);
	Oled_Command_Write(0x07);

	volatile char *sram_read = (char *) (0x1800);
	volatile char *oled_write = (char *) 0x1200;

	for (uint16_t i = 0; i < 0x400; i++)
	{
		*oled_write = sram_read[i];
	}
}