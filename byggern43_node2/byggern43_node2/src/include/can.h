/*
 * can.h
 *
 * Created: 04.10.2019 16:33:36
 *  Author: henridah
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <stdio.h>

typedef struct can_data_t {
	uint8_t id;
	uint8_t length;
	uint8_t data[8];
} can_data_t;

void Can_Init(void);
void Can_Send_Msg(can_data_t* data);
void Can_Recieve_Msg(can_data_t* data);
void Can_Handle_Data(void);
uint8_t Get_Shoot_Solenoid_Status(void);
void set_shoot_solenoid_status(uint8_t state);
void Set_Slide_Pos(uint8_t pos);

#endif /* CAN_H_ */