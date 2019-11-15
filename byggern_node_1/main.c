/*
 * byggern_node_1.c
 *
 * Created: 15.11.2019 08.30.38
 * Author : hnd00
 */
#include "sources/include/settings.h"

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "sources/include/usercontrol.h"
#include "sources/include/oled.h"
#include "sources/include/usart.h"
#include "sources/include/timer.h"
#include "sources/include/can.h"

int main(void)
{
	USART_Init(MYUBRR);
    Timer_Init();
	User_Control_Init();
    Can_Init();
    while (1)
    {
    }
}

// main timer
// might need a counter

ISR(TIMER0_OVF_vect){
	Oled_Update();
    Menu_Navigation();
    Menu_Functionality();
}

ISR(TIMER3_COMPA_vect){

}