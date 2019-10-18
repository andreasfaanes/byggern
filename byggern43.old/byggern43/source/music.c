/*
 * music.c
 *
 * Created: 18.10.2019 09:56:25
 *  Author: henridah
 */ 

#include "avr/io.h"
#include "include/timer.h"
#include "include/music.h"
#include <stdio.h>

const uint16_t PROGMEM melody[78] = {
	NOTE_E7, NOTE_E7, 0, NOTE_E7,
	0, NOTE_C7, NOTE_E7, 0,
	NOTE_G7, 0, 0,  0,
	NOTE_G6, 0, 0, 0,
	
	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,
	
	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0,
	
	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,
	
	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
const uint16_t PROGMEM tempo[78] = {
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
};
/*
//Underworld melody
int underworld_melody[] = {
	NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
	NOTE_AS3, NOTE_AS4, 0,
	0,
	NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
	NOTE_AS3, NOTE_AS4, 0,
	0,
	NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
	NOTE_DS3, NOTE_DS4, 0,
	0,
	NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
	NOTE_DS3, NOTE_DS4, 0,
	0, NOTE_DS4, NOTE_CS4, NOTE_D4,
	NOTE_CS4, NOTE_DS4,
	NOTE_DS4, NOTE_GS3,
	NOTE_G3, NOTE_CS4,
	NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
	NOTE_GS4, NOTE_DS4, NOTE_B3,
	NOTE_AS3, NOTE_A3, NOTE_GS3,
	0, 0, 0
};
//Underworld tempo
int underworld_tempo[] = {
	12, 12, 12, 12,
	12, 12, 6, 3,
	12, 12, 12, 12,
	12, 12, 6, 3,
	12, 12, 12, 12,
	12, 12, 6, 3,
	12, 12, 12, 12,
	12, 12, 6,
	6, 18, 18, 18,
	6, 6,
	6, 6,
	6, 6,
	18, 18, 18, 18, 18, 18,
	10, 10, 10,
	10, 10, 10,
	3, 3, 3
};
*/
void music_init(void){
	timer_init();
}

void music_start(void){
	timer_1_on();
	timer_3_on();
}

void music_stop(void){
	timer_1_off();
	timer_3_off();
}

void music_play_note(uint16_t note, uint16_t tempo){
	timer_1_set_top(note);
	timer_3_set_top(tempo*10000);
}




uint16_t get_note(uint8_t note) {
	return pgm_read_byte(&melody[note]);
}

uint16_t get_tempo(uint8_t note){
	return pgm_read_byte(&tempo[note]);
}