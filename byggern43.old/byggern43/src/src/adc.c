#include "settings.h"
#include "adc.h"
#include <stdio.h>



void adc_init(){
	DDRE &= ~(1<<PINE0);
}

uint8_t adc_read(uint8_t channel){
	volatile char *ext_adc = (char * ADC_MEM_START);
	
	if (channel > 3){return NULL};
	
	//check if it works right!
	*ext_adc = 0x04|channel;
	
	while (TEST_BIT(PINE, PINE0));
	
	return *ext_adc;
}