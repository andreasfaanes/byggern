/*
 * music.h
 *
 * Created: 18.10.2019 09:56:40
 *  Author: henridah
 */ 

#include "settings.h"
#include <avr/pgmspace.h>

#ifndef MUSIC_H_
#define MUSIC_H_

#define buzzer_pin (1 << PINB5)

/////////////////////////////////////////////////
//note
#define NOTE_E7 116

#define NOTE_G7 97

#define NOTE_G6 195

#define NOTE_E6 232

#define NOTE_A6 174

#define NOTE_B6 155

#define NOTE_AS6 164

#define NOTE_A7 87

#define NOTE_D7 130

#define NOTE_F7 109

#define NOTE_C4 1172

#define NOTE_C5 587

#define NOTE_A3 1396

#define NOTE_A4 698

#define NOTE_AS3 1318

#define NOTE_AS4 659

#define NOTE_F3 1755

#define NOTE_F4 880

#define NOTE_DS3 1969

#define NOTE_DS4 987

#define NOTE_D3 1956

#define NOTE_D4 1044

#define NOTE_CS4 1109

#define NOTE_GS3 1476

#define NOTE_G3 1567

#define NOTE_B3 1243

#define NOTE_C7 146

#define NOTE_FS4 830

#define NOTE_E3 1861

#define NOTE_GS4 740


#define TEMPO_CONST ((60*F_OSC)/(1000*256)
#define MELODY_LENGTH 78


void music_start(void);
void music_init(void);
void music_play_note(uint16_t note, uint16_t tempo);

uint16_t get_tempo(uint8_t note);
uint16_t get_note(uint8_t note);

#endif /* MUSIC_H_ */