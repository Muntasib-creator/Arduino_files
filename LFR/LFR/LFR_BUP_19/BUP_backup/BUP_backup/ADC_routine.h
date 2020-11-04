#ifndef ADC_routine
#define ADC_routine

#include <avr/io.h>
#include <inttypes.h>

void adc_init();

int adcRead(uint8_t channel);

#endif
