/*
 * pwm.h
 *
 * Created: 18.10.2019 13:10:46
 *  Author: henridah
 */


#ifndef TIMER_H_
#define TIMER_H_

/**
 *  Init for the timers used in this project
 **/
void Timer_Init(void);

// Timer 0 used for:

// Timer 1 used for:
void Timer_1_on(void);
void Timer_1_off(void);
void Timer_1_set_top(uint16_t top_val);

// Timer 2 used for: Nothing

// Timer 3 used for:
void Timer_3_on(void);
void Timer_3_off(void);
void Timer_3_Set_Top(uint16_t top_val);
#endif /* TIMER_H_ */