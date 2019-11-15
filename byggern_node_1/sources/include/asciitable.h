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
void Ascii_to_Bitmap_Font4(char* OledCharacter, char AsciiCharacter);

void Ascii_to_Bitmap_Font5(char* OledCharacter, char AsciiCharacter);

void Ascii_to_Bitmap_Font8(char* OledCharacter, char AsciiCharacter);

void Ascii_to_Bitmap_Custom(char* OledCharacter, char AsciiCharacter);

char Lunde_Byte(uint16_t byte);

#endif /* ASCIITABLE_H_ */