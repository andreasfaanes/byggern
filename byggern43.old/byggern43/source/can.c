/*
 * can.c
 *
 * Created: 04.10.2019 16:33:24
 *  Author: henridah
 */ 

#include "include/can.h"
#include "include/spi.h"
#include "include/MCP2515.h"
#include <avr/io.h>
#include <stdio.h>

void can_init(void){
	spi_init();
	mcp_reset();
	mcp_modify_bit(MCP_CANCTRL,0xff,MODE_CONFIG);
	
	mcp_modify_bit(MCP_RXB0CTRL,0x60,0xff);
	
	mcp_modify_bit(MCP_CANINTE,0x01,0x01);
	mcp_modify_bit(MCP_CANCTRL, MODE_MASK,MODE_NORMAL);
	
	DDRD &= ~(1<<PD2);

	GICR |= (1 << INT0);
}

void can_send_msg(can_data_t* data){
	mcp_write(MCP_TXB0SIDH,data->id / 0b1000);
	mcp_write(MCP_TXB0SIDL,(data->id % 0b1000) << 5);
	mcp_write(MCP_TXB0DLC,data->length);
	
	for (uint8_t i = 0; i < data->length; i++)
	{
		mcp_write(MCP_TXB0D0+i,data->data[i]);
	}
	
	mcp_request_to_send(0); //1
}

void can_recieve_msg(can_data_t* data){
	
	if (mcp_read(MCP_CANINTF) & 0x01){
		printf("VALID MESSAGE \n\r");
		uint8_t idhigh = mcp_read(MCP_RXB0SIDH);
		uint8_t idlow = mcp_read(MCP_RXB0SIDL);
		data->id = (idhigh << 3)|(idlow >> 5);
		
		data->length = mcp_read(MCP_RXB0DLC) & 0x0f;
		
		for (uint8_t i = 0; i < data->length; i++)
		{
			data->data[i] = mcp_read(MCP_RXB0D0+i);
		}
		mcp_modify_bit(MCP_CANINTF, 0b01, 0);
	}
}