/*
 * encoder.h
 *
 * Created: 01.11.2019 13:18:38
 *  Author: vebjornt
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_
int16_t Encoder_Read_Data(void);

void Encoder_Calibrate(void);
void Print_Pos(void);

#endif /* ENCODER_H_ */