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

typedef enum can_id{
	slider_id = 1,
	joycon_id
} can_id_t;

void Can_Init(void);
void Can_Send_Msg(can_data_t* data);
void Can_Recieve_Msg(can_data_t* data);


#endif /* CAN_H_ */