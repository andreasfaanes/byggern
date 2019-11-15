/*
 * oled.h
 *
 * Created: 20.09.2019 12:54:03
 *  Author: andrefaa
 */ 

#ifndef OLED_H_
#define OLED_H_

#include <stdio.h>

#define FONT_SIZE 5

#define OLED_SET_MEMORY_ADDR_MODE 0x20
#define OLED_SET_COLUMN_ADDR_RANGE 0x21
#define OLED_SET_PAGE_ADDR 0x22
#define OLED_SET_CONTRAST_CONTR	0x81
#define OLED_SET_SEGMENT_REMAP 0xa0
#define OLED_ENTIRE_DISP_ON 0xa4
#define OLED_SET_NORMAL_DISPLAY 0xa6
#define OLED_SET_MUX_RATIO 0xa8
#define OLED_EXT_IREF_SELECTION 0xad
#define OLED_SET_DISPLAY_ON	0xae
#define OLED_SET_PAGE_START_FOR_ADDR 0xb0
#define OLED_SET_COM_OUTPUT_SCAN_DIR 0xc0
#define OLED_SET_DISPLAY_OFFSET 0xd3
#define OLED_SET_DISPLAY_CLOCK 0xd5
#define OLED_SET_PRE_CHARGE_PERIOD 0xd9
#define OLED_SET_COM_PINS_HW_CONF 0xda
#define OLED_SET_VCOMG_DESELECT_LEVEL 0xdb
#define OLED_NOP 0xe3

void Oled_Init(void);
void Oled_Reset(void);
void Oled_Home(void);
void Oled_Goto_Line(uint8_t line);
void Oled_Clear_Line(uint8_t line);
void Oled_Pos(uint8_t row);
void Oled_Print_char(char character,uint8_t inverted, uint8_t column);
void Oled_Print(char* characters,uint8_t inverted);
void Oled_Set_Brightness(uint8_t lvl);
void Oled_Data_Write(uint8_t data, uint8_t column);
void Oled_Update(void);
void Oled_Print_Custom(char character,uint8_t column);
void Print_Lunde(void);

#endif /* OLED_H_ */