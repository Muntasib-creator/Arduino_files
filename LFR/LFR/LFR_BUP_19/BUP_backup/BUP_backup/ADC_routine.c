#include "ADC_routine.h"
#include "serial_lib.h"

void adc_init()
{
	ADMUX |= (1 << REFS0);									    //reference AVcc 5volt
	ADCSRA |= (1 << ADEN);										//enable ADC Module
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);		//prescaler 128
}

int adcRead(uint8_t channel)
{
	ADMUX  &= ~(0x1F);				//disable all ADC channel
	ADMUX |= channel;		//set the ADC channel
	ADCSRA |= (1<<ADSC);			//start ADC conversion
	while(ADCSRA & (1<<ADSC));		//wait until conversion complete
	return ADC;						//return the ADC converted digital value
}
