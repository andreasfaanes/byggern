#include "include/settings.h"
#include "include/adc.h"
#include <stdio.h>
#include <avr/io.h>



void Adc_Init(){
	DDRE &= ~(1<<PINE0);
}

uint8_t Adc_Read(uint8_t channel){
	volatile char *ext_adc = (char *) ADC_MEM_START;

	if (channel > 3){return -1;}

	//check if it works right!
	*ext_adc = 0x04|channel;

	while (TEST_BIT(PINE, PINE0));

	return *ext_adc;
}