/*
 * spi.h
 *
 * Created: 04.10.2019 09:26:40
 *  Author: henridah
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>

void spi_init(void);
void spi_write(uint8_t data);
uint8_t spi_read(void);
void slave_enable(void);
void slave_deselect(void);

#endif /* SPI_H_ */