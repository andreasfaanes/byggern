#include <stdio.h>
#include <avr/io.h>
#include "include/settings.h"
#include "include/ext_mem.h"

void Ext_Mem_Init(){
	MCUCR |= (1<< SRE);
	SFIOR |= (1<< XMM2);
}