/*
 * MCP2515.c
 *
 * Created: 04.10.2019 10:16:05
 *  Author: henridah
 */ 

#include "MCP2515.h"
#include "spi.h"
#include <avr/interrupt.h>
#include <avr/io.h>

uint8_t mcp_read(uint8_t addr){
	uint8_t result;
	
	slave_enable();
	
	spi_write(MCP_READ);
	spi_write(addr);
	result = spi_read();
	
	slave_deselect();
	return result;
}

void mcp_write(uint8_t addr, uint8_t data){
	slave_enable();
	spi_write(MCP_WRITE);
	spi_write(addr);
	spi_write(data);
	slave_deselect();
}

void mcp_reset(void){
	slave_enable();
	spi_write(MCP_RESET);
	slave_deselect();
}

void mcp_modify_bit(uint8_t addr, uint8_t mask, uint8_t data){
	slave_enable();
	spi_write(MCP_BITMOD);
	spi_write(addr);
	spi_write(mask);
	spi_write(data);
	slave_deselect();
}

void mcp_request_to_send(uint8_t buffer){
	slave_enable();
	spi_write(0x80 | (1<< buffer));
	slave_deselect();
}



ISR(INT0_vect){
}

/*
	endre alt som heter can_read til f.eks mcp2515_read
	
	trenger ingen init for mcp2515, heller lag et eget can-bibliotek med can_init, can_send og can_receive

*/