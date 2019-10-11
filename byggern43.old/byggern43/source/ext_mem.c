#include <stdio.h>
#include <avr/io.h>
#include "include/settings.h"
#include "include/ext_mem.h"

void ext_mem_init(){
	MCUCR |= (1<< SRE);
	SFIOR |= (1<< XMM2);	
}