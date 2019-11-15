#include <stdio.h>

/**
*   Initialization of ADC
*
**/
void Adc_Init();

/**
 *  Read value from adc
 *
 *  returns a value of uint8_t {0 -> 255}
 *
 *  channel, selects the channel to read from {0, 1, 2, 3}
 */
uint8_t Adc_Read(uint8_t channel);