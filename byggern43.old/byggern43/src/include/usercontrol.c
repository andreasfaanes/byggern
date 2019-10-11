#include "usercontrol.h"
#include "adc.h"
#include "ext_mem.h"

#include <stdio.h>
#include <avr/io.h>


void user_control_init(){
		ext_mem_init();
		adc_init();
		
		//enabels buttons as inputs
		DDRB |= (1 << DDB0)|(1<<DDB1)|(1<<DDB2);
		
		//sets pull ups high so joystick push to work
		PORTB |= (1<<PINB2);
}



joycon_t joycon_get(){
	joycon_t joycon;
	uint8_t x,y;
	
	x = adc_read(0);
	y = adc_read(1);
	
	if (x > JOYCON_MID)
	{
		joycon.x = 100*(x-JOYCON_MID)/(0xFF-JOYCON_MID);
	}else if (x < JOYCON_MID)
	{
		joycon.x = 100*(x-JOYCON_MID)/(JOYCON_MID);
	}else
	{
		joycon.x = 0;
	}
	
	if (y > JOYCON_MID)
	{
		joycon.y = 100*(y-JOYCON_MID)/(0xFF-JOYCON_MID);
	}else if (y < JOYCON_MID)
	{
		joycon.y = 100*(y-JOYCON_MID)/(JOYCON_MID);
	}else
	{
		joycon.y = 0;
	}
	
	return joycon;
}