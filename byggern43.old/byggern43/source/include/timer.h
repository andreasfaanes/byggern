/*
 * pwm.h
 *
 * Created: 18.10.2019 13:10:46
 *  Author: henridah
 */ 


#ifndef TIMER_H_
#define TIMER_H_


void timer_init(void);

void timer_1_on(void);
void timer_1_off(void);
void timer_1_set_top(uint16_t top_val);

void timer_3_on(void);
void timer_3_off(void);
void timer_3_set_top(uint16_t top_val);
#endif /* PWM_H_ */