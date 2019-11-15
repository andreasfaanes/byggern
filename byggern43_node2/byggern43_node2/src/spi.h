/*
 * spi.h
 *
 * Created: 04.10.2019 09:26:40
 *  Author: henridah
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>

void Spi_Init(void);
void Spi_Write(uint8_t data);
uint8_t Spi_Read(void);
void Slave_Enable(void);
void Slave_Deselect(void);

#endif /* SPI_H_ */