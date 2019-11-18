/*
 * can.c
 *
 * Created: 04.10.2019 16:33:24
 *  Author: henridah
 */ 

#include "include/can.h"
#include "include/spi.h"
#include "include/MCP2515.h"
#include "include/PID.h"
#include "include/solenoid.h"
#include "include/timer.h"
#include "include/settings.h"
#include "include/encoder.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int16_t slider_pos = 250;//Sets default to middle
int16_t pos = 0;
uint8_t shoot_solenoid_status = 0;
can_data_t data;

void Can_Init(void){
	Spi_Init();
	Mcp_Reset();
	Mcp_Modify_Bit(MCP_CANCTRL,0xff,MODE_CONFIG);
	
	Mcp_Modify_Bit(MCP_RXB0CTRL,0x60,0xff);
	
	Mcp_Modify_Bit(MCP_CANINTE,0x01,0x01);
	Mcp_Modify_Bit(MCP_CANCTRL, MODE_MASK,MODE_NORMAL);
	
	//DDRD &= ~(1<<PD2);

	//GICR |= (1 << INT0);
}

void Can_Send_Msg(can_data_t* data){
	//fetch out MSB amd LSB from the id
	Mcp_Write(MCP_TXB0SIDH,data->id / 0b1000);
	Mcp_Write(MCP_TXB0SIDL,(data->id % 0b1000) << 5);
	
	Mcp_Write(MCP_TXB0DLC,data->length);
	printf("Data with ID is sent %u   \n\r",data->id);
	for (uint8_t i = 0; i < data->length; i++)
	{
		Mcp_Write(MCP_TXB0D0+i,data->data[i]);
	}
	
	Mcp_Request_to_Send(0); //1
}

void Can_Recieve_Msg(can_data_t* data){
	//checks if msg arrived
	if (Mcp_Read(MCP_CANINTF) & 0x01){
		
		
		uint8_t idhigh = Mcp_Read(MCP_RXB0SIDH);
		uint8_t idlow = Mcp_Read(MCP_RXB0SIDL);
		//organized idhigh as MSB, idlow as LSB
		data->id = (idhigh << 3)|(idlow >> 5);
		printf("Data with ID is recieved %u \n\r",data->id);
		data->length = Mcp_Read(MCP_RXB0DLC) & 0x0f;
		
		for (uint8_t i = 0; i < data->length; i++)
		{
			data->data[i] = Mcp_Read(MCP_RXB0D0+i);
		}
		Mcp_Modify_Bit(MCP_CANINTF, 0b01, 0);
	}
}

void Can_Handle_Data(void){

	pos = Encoder_Read_Data();
	Set_Power_and_Direction(Calculate_PID_Power(slider_pos, pos));
}

uint8_t Get_Shoot_Solenoid_Status(void)
{
	return shoot_solenoid_status;
}

uint8_t counter = 0;
ISR(TIMER3_COMPA_vect){
	Can_Handle_Data();
	
	if (Get_Shot_Recently()){
		if (counter > 15) {
			Set_Shot_Recently(0);
			counter = 0;
		}
		counter ++;
	}
}
void Set_Slide_Pos(uint8_t pos){
	slider_pos = pos;
}
void set_shoot_solenoid_status(uint8_t state){
	shoot_solenoid_status = state;
}