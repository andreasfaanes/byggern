/*
 * encoder.h
 *
 * Created: 01.11.2019 13:18:38
 *  Author: vebjornt
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_
int16_t encoder_read_data(void);

void encoder_calibrate(void);
void print_pos(void);

#endif /* ENCODER_H_ */