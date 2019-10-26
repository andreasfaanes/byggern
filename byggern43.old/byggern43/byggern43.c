/*
 * byggern43.c
 *
 * Created: 30.08.2019 10:09:51
 *  Author: andrefaa
 */ 
#include "src/include/usart.h"
#include "source/include/settings.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "source/include/ext_mem.h"
#include "source/include/adc.h"
#include "source/include/usercontrol.h"
#include "source/include/oled.h"
#include "source/include/MCP2515.h"
#include "source/include/can.h"
#include "source/include/music.h"
#include <stdlib.h>



// calls when timer overflows

ISR(TIMER0_OVF_vect){
	oled_update();
}

void int_timer_setup(void){

	
}

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size= 0x800;
	uint16_t write_errors= 0;
	uint16_t retrieval_errors= 0;
	printf("Starting SRAM test...\n");
	// rand() stores someinternal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before thisfunction)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i,retreived_value, some_value);
			write_errors++;
		}
		//_delay_ms(500);
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r",i, retreived_value, some_value);
			retrieval_errors++;
		}
		//_delay_ms(500);
	}
	printf("SRAM test completed with\n%4d errors in write phase and\n%4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
}

joycon_t joycon;
slider_postion_t slider_pos;
		
volatile char *ext_oled_data = (char *) OLED_DATA_MEM_START;
volatile char *ext_oled_com = (char *) OLED_COM_MEM_START;
void test_function_joycon_setup(void){

}

void test_function_joycon_loop(void){
	joycon = joycon_get_direction();
	slider_pos = slider_get_postion();
	printf("x = %i,				y = %i				dir = %u \n\r", joycon.x,joycon.y, joycon.direction);
	printf("LEFT = %i,				RIGHT = %i				\n\r", slider_pos.left_slider,slider_pos.right_slider);
	
}

// main function
int main(void)
{
	
	USART_init(MYUBRR);
	DDRD |= (1 << PIND5);
	printf("start!\n\r");
	user_control_init();
	cli();
	oled_menu_setup();
	can_init();
	music_init();
	//oled_reset();
	//oled_update();
	music_start();
	
	sei(); // global interupt enable
	printf("Her? \n\r");
	while(1)
	{	
		//PORTD |= (1 << PIND5);
		menu_navigation();
		slider_send_pos();
		//_delay_ms(300);
		
		//print_lunde();
		//printf("done");
	}
}

volatile uint8_t note = 0;

ISR(TIMER3_COMPA_vect){
	//music_play_note(get_note(note),get_tempo(note));
	if (note >= MELODY_LENGTH)
	{
		note = 0;
	}else{
		note ++;
	}
	
}
