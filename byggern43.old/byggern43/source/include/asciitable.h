/*
 * asciitable.h
 *
 * Created: 20.09.2019 14:14:11
 *  Author: andrefaa
 */ 


#ifndef ASCIITABLE_H_
#define ASCIITABLE_H_

#include <avr/pgmspace.h>

/**
* brief: function for converting ascii to bit map
* 
* input: oledcharacter returns bit map
*		 asciiCharacter character to convert
*		 font_size size of font, can be 4, 5 or 8
*/
void ascii_to_bitmap_font4(char* OledCharacter, char AsciiCharacter);

void ascii_to_bitmap_font5(char* OledCharacter, char AsciiCharacter);

void ascii_to_bitmap_font8(char* OledCharacter, char AsciiCharacter);

void ascii_to_bitmap_custom(char* OledCharacter, char AsciiCharacter);

char lunde_byte(uint16_t byte);

#endif /* ASCIITABLE_H_ */