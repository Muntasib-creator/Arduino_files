#ifndef PWM_routine
#define PWM_routine

#include <avr/io.h>

void PWM0_init(void);
void PWM1_init(void);
void PWM2_init(void);

void PWM0_terminate(void);
void PWM1_terminate(void);
void PWM2_terminate(void);

void PWM0(int dutyCycle);
void PWM1A(int dutyCycle);
void PWM1B(int dutyCycle);
void PWM2(int dutyCycle);


#endif