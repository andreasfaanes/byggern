#include <stdio.h>
#include "settings.h"
#include "ext_mem.h"

void ext_mem_init(){
	MCUCR |= (1<< SRE);
	SFIOR |= (1<< XMM2);	
}