#include "PWM_routine.h"



/* PWM SETTINGS*/
		
void PWM0_init(void)										// PWM0 (PB3 of ATmega32A)
{
	TCCR0 |= (1 << WGM00) | (1 << WGM01);					// Enable Fast PWM
	TCCR0 |= (1 << COM01);									// Clear OC2 on Compare Match (non-inverting mode)
	TCCR0 |= (1 << CS00);									// Set prescaler 1	
}



void PWM1_init(void)										// PWM1A (PD5 of ATmega32A) and PWM0 (PB3 of ATmega32A)
{
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10);	
}



void PWM2_init(void)										// PWM2 (PD7 of ATmega32A)
{
	TCCR2 |= (1 << WGM20) | (1 << WGM21);					// Enable Fast PWM
	TCCR2 |= (1 << COM21);									// Clear OC2 on Compare Match (non-inverting mode)
	TCCR2 |= (1 << CS20);									// Set prescaler 1	
}

void PWM0_terminate(void)
{
	TCCR0 = 0x00;
}

void PWM1_terminate(void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x00;
}

void PWM2_terminate(void)
{
	TCCR2 = 0x00;
}




/* PWM Controlling Functions*/

void PWM0(int dutyCycle)
{
	OCR0 = dutyCycle;
}

void PWM1A(int dutyCycle)
{
	OCR1A = dutyCycle;
}

void PWM1B(int dutyCycle)
{
	OCR1B = dutyCycle;
}

void PWM2(int dutyCycle)
{
	OCR2 = dutyCycle;
}
