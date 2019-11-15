/*
 * MCP2515.c
 *
 * Created: 04.10.2019 10:16:05
 *  Author: henridah
 */

#include "include/MCP2515.h"
#include "include/spi.h"
#include <avr/interrupt.h>
#include <avr/io.h>

uint8_t Mcp_Read(uint8_t addr){
	uint8_t result;

	Slave_Enable();

	Spi_Write(MCP_READ);
	Spi_Write(addr);
	result = Spi_Read();

	Slave_Deselect();
	return result;
}

void Mcp_Write(uint8_t addr, uint8_t data){
	Slave_Enable();
	Spi_Write(MCP_WRITE);
	Spi_Write(addr);
	Spi_Write(data);
	Slave_Deselect();
}

void Mcp_Reset(void){
	Slave_Enable();
	Spi_Write(MCP_RESET);
	Slave_Deselect();
}

void Mcp_Modify_Bit(uint8_t addr, uint8_t mask, uint8_t data){
	Slave_Enable();
	Spi_Write(MCP_BITMOD);
	Spi_Write(addr);
	Spi_Write(mask);
	Spi_Write(data);
	Slave_Deselect();
}

void Mcp_Request_to_Send(uint8_t buffer){
	Slave_Enable();
	Spi_Write(0x80 | (1<< buffer));
	Slave_Deselect();
}



ISR(INT0_vect){
}
