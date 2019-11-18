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

void Can_Init(void){
	Spi_Init();
	Mcp_Reset();
	Mcp_Modify_Bit(MCP_CANCTRL,0xff,MODE_CONFIG);

	Mcp_Modify_Bit(MCP_RXB0CTRL,0x60,0xff);

	Mcp_Modify_Bit(MCP_CANINTE,0x01,0x01);
	Mcp_Modify_Bit(MCP_CANCTRL, MODE_MASK,MODE_NORMAL);

	DDRD &= ~(1<<PD2);

	GICR |= (1 << INT0);
}

void Can_Send_Msg(can_data_t* data){
	Mcp_Write(MCP_TXB0SIDH,data->id / 0b1000);
	Mcp_Write(MCP_TXB0SIDL,(data->id % 0b1000) << 5);
	Mcp_Write(MCP_TXB0DLC,data->length);
	printf("SENDT VALID MESSAGE id: %u\n\r", data->id);
	for (uint8_t i = 0; i < data->length; i++)
	{
		Mcp_Write(MCP_TXB0D0+i,data->data[i]);
	}
	
	Mcp_Request_to_Send(0); //1
}

void Can_Recieve_Msg(can_data_t* data){

	if (Mcp_Read(MCP_CANINTF) & 0x01){
		
		uint8_t idhigh = Mcp_Read(MCP_RXB0SIDH);
		uint8_t idlow = Mcp_Read(MCP_RXB0SIDL);
		data->id = (idhigh << 3)|(idlow >> 5);
		printf("VALID MESSAGE recieved with id: %u\n\r", data->id);
		data->length = Mcp_Read(MCP_RXB0DLC) & 0x0f;

		for (uint8_t i = 0; i < data->length; i++)
		{
			data->data[i] = Mcp_Read(MCP_RXB0D0+i);
		}
		Mcp_Modify_Bit(MCP_CANINTF, 0b01, 0);
	}
}