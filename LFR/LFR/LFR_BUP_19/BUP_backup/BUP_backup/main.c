/*
 * BUP_2019.c
 *
 * ATmega32A, 3 Sonar, 8 IR sensor array
 * Fuse bits: Low Fuse: 3F, High Fuse: C9
 * Clock Frequency: 16 MHz *
 *
 * Created: 05-Apr-19 9:06:06 AM
 * Author : FARHAN FUAD
 */ 

#define F_CPU 16000000UL

#define trig1_DDR				DDRD
#define trig1_PORT				PORTD
#define trig1_PIN				4
#define trig2_DDR				DDRD
#define trig2_PORT				PORTD
#define trig2_PIN				5
#define trig3_DDR				DDRB
#define trig3_PORT				PORTB
#define trig3_PIN				1
#define echo1_DDR				DDRD
#define echo1_PORT				PORTD
#define echo1_PIN				3
#define echo2_DDR				DDRD
#define echo2_PORT				PORTD
#define echo2_PIN				2
#define echo3_DDR				DDRB
#define echo3_PORT				PORTB
#define echo3_PIN				2

#define motor1a_DDR				DDRC
#define motor1a_PORT			PORTC
#define motor1a_PIN				7
#define motor1b_DDR				DDRC
#define motor1b_PORT			PORTC
#define motor1b_PIN				6
#define motor2a_DDR				DDRC
#define motor2a_PORT			PORTC
#define motor2a_PIN				5	
#define motor2b_DDR				DDRC
#define motor2b_PORT			PORTC
#define motor2b_PIN				4
#define enable1_DDR				DDRD
#define enable1_PORT			PORTD
#define enable1_PIN				7
#define enable2_DDR				DDRB
#define enable2_PORT			PORTB
#define enable2_PIN				3

//#define maxSpeed				180
#define setPoint				5

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>
#include "serial_lib.h"
#include "ADC_routine.h"
#include "PWM_routine.h"

#define enable1					PWM0
#define enable2					PWM2

#define	speedCoefficient		5.3


//uint16_t threshold[8] = {700, 700, 700, 700, 700, 700, 700, 700};
uint16_t threshold[8] = {387, 352, 441, 422, 396, 427, 309, 323};
uint16_t upperThreshold[8] = {547, 684, 618, 705, 679, 663, 723, 636};
uint16_t lowerThreshold[8] = {369, 486, 432, 497, 475, 459, 503, 438};
	
uint16_t proportionality[8] = {0, 0, 0, 0, 0, 0, 0, 0};

uint16_t SensorMaxValue[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t SensorMinValue[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};

uint16_t prev_SensorValue[8] = {0, 0, 0, 0, 0, 0, 0, 0};

uint8_t sonar1_val[5], sonar2_val[5], sonar2_val[5];
uint8_t sonar1_readingFiltered = 0;
uint8_t sonar2_readingFiltered = 0;
uint8_t sonar3_readingFiltered = 0;
uint8_t sonar1_turn = 0;
uint8_t sonar2_turn = 0;
uint8_t sonar3_turn = 0;

uint16_t timerOverflow = 0;
uint16_t duration = 0;
uint8_t timerFlag = 0;
uint16_t flashcount = 0;

float error = 0;
float prev_error = 0;
int16_t sum_error = 0;
float correction = 0;
int16_t motorResponse = 0;
int16_t LMspeed = 0; 
int16_t RMspeed = 0;
uint8_t reverseSpeed = 0;
uint8_t weightedValue = 0;
uint8_t sensorCount = 0;
char flashMode = 0;
int stopFlag = 0;
uint8_t rightFlag = 0, leftFlag = 0, allFlag = 0;
uint8_t rightAcuteFlag = 0, leftAcuteFlag = 0;

uint8_t sonar1_ack = 0;
uint8_t sonar2_ack = 0;
uint8_t sonar3_ack = 0;
uint16_t pulse1 = 0;
uint16_t pulse2 = 0;
uint16_t pulse3 = 0;
uint8_t distance1 = 0;
uint8_t distance2 = 0;
uint8_t distance3 = 0;

const uint8_t maxSpeed = 255;
const uint8_t baseSpeed = 150;
uint16_t Kp = 75; //70;		// 70 @ rmax 255
uint16_t Kd = 250; //150; //250;		// 250 @ rmax 255

void setTimer(uint16_t durationMicros);
void timercount(uint16_t durationmillis);
void timerOff(void);
void autoCalibration(void);
void sensorValueConversion(void);
void sensorValuePrint(void);
void sensorMapping(void);
void PID_Correction(void);
void exceptionHandling (void);

uint8_t medianFilter(uint8_t data[], uint8_t n);
void distanceMeasure(void);
void sonarGuideCave(void);
void TIMER0_2_INT_init(void);
void xINT_init(void);
void InterruptOff(void);

void Forward(void);
void Forward_slow(void);
void Backward(void);
void Stop(void);
void RightSharp(uint8_t motorSpeed);
void LeftSharp(uint8_t motorSpeed);
void setRotationForward(void);
void setRotationRightSharp(void);
void setRotationLeftSharp(void);

uint16_t sonar1Read(void);
uint16_t sonar2Read(void);


#define sonarSetPoint  8

int main(void)
{
	// Input-Output settings
	trig1_DDR |= (1 << trig1_PIN);
	trig2_DDR |= (1 << trig2_PIN);
	trig3_DDR |= (1 << trig3_PIN);
	echo1_DDR &= ~(1 << echo1_PIN);
	echo2_DDR &= ~(1 << echo2_PIN);
	echo3_DDR &= ~(1 << echo3_PIN);
	motor1a_DDR |= (1 << motor1a_PIN);
	motor1b_DDR |= (1 << motor1b_PIN);
	motor2a_DDR |= (1 << motor2a_PIN);
	motor2b_DDR |= (1 << motor2b_PIN);
	enable1_DDR |= (1 << enable1_PIN);
	enable2_DDR |= (1 << enable2_PIN);	
	
// 	uint8_t sKp = 40;
// 	uint16_t sKd = 250;
// 	
// 	int8_t prev_sonarError = 0;
// 	int8_t sonarError = 0;
//	int sonarCorrection = 0;

	// Initializations	
	
	adc_init();
	UART_init();
	PWM0_init();
	PWM2_init();
	setRotationForward();
	
//	Forward();					//////trial
//	_delay_ms(10000);			//////trial
	
	_delay_ms(1000);
	autoCalibration();
	_delay_ms(1000);
	
	Forward();

// 	enable1(maxSpeed);
// 	enable2(maxSpeed - (maxSpeed / 5.3));
    while (1) 
    {		
		sensorValueConversion();
//		sensorValuePrint();

/*****************************************************Object Avoiding****************************************************/

		sonar2_val[sonar2_turn] = sonar2Read();
		if (sonar2_turn >= 4)
		{
			sonar2_turn = 0;
			uint8_t i = 0, j = 0, key = 0;

			for(i = 1; i < 5; i++)
			{
				key = sonar2_val[i];
				j = i - 1;
				while(j >= 0 && sonar2_val[j] > key)
				{
					sonar2_val[j + 1] = sonar2_val[j];
					j--;
				}
				sonar2_val[j + 1] = key;
			}

			distance2 = sonar2_val[4];
		}
		else sonar2_turn++;


		if (distance2 < 25 && sensorCount == 0/* error < 2 && error > -2*/ )
		{
			Backward();
			_delay_ms(100);
			Stop();
// 			while (distance2 < 12)
// 			{
// 				sonar2_val[sonar2_turn] = sonar2Read();
// 				if (sonar2_turn >= 4)
// 				{
// 					sonar2_turn = 0;
// 					uint8_t i = 0, j = 0, key = 0;
// 
// 					for(i = 1; i < 5; i++)
// 					{
// 						key = sonar2_val[i];
// 						j = i - 1;
// 						while(j >= 0 && sonar2_val[j] > key)
// 						{
// 							sonar2_val[j + 1] = sonar2_val[j];
// 							j--;
// 						}
// 						sonar2_val[j + 1] = key;
// 					}
// 
// 					distance2 = sonar2_val[2];
// 				}
// 				else sonar2_turn++;
// 			}

			RightSharp(120);
			_delay_ms(600);
			Forward_slow();
			_delay_ms(900);
			LeftSharp(120);
			_delay_ms(600);
			Forward_slow();
			_delay_ms(1300);
			LeftSharp(120);
			_delay_ms(450);
			Forward_slow();
			_delay_ms(800);
			sensorValueConversion();
			while(sensorCount == 0) sensorValueConversion();
		}
/*****************************************************Wall Following****************************************************/

		if (sensorCount == 0)
		{
			if (bit_is_clear(PIND, 3))
			{
				RightSharp(140);
				while (bit_is_clear(PIND, 3));
				Forward_slow();
			}
			else if (bit_is_clear(PINB, 2))
			{
				LeftSharp(140);
				while (bit_is_clear(PINB, 2));
				Forward_slow();
			}
		}
		
/*****************************************************Line Follow Turns****************************************************/

		if(leftFlag == 1 && weightedValue < 7 && weightedValue > 3)
		{
			Forward();
			while (leftFlag == 1)
			{
				sensorValueConversion();
				if (allFlag == 1 || rightFlag == 1) break;
			}
			if (allFlag == 0 && rightFlag == 0)
			{
				_delay_ms(100);							// wait till all white
				sensorValueConversion();				// check if there is any line
				if (sensorCount == 0 && rightFlag == 0)
				{
					Backward();
					_delay_ms(300);
					LeftSharp(120);
					_delay_ms(100);
					while (sensorCount == 0 || weightedValue < 5) sensorValueConversion();
				}
			}
		}

		else if(rightFlag == 1 && weightedValue < 7 && weightedValue > 3)
		{
			Forward();
			while (rightFlag == 1)
			{
				sensorValueConversion();
				if (allFlag == 1 || leftFlag == 1) break;
			}
			if (allFlag == 0 && leftFlag == 0)
			{
// 				_delay_ms(150);
// 				sensorValueConversion();
// 				RightSharp(160);
// 				if (sensorCount == 0 && leftFlag == 0)
// 				{
// 					Backward();
// 					_delay_ms(350);
// 					RightSharp(120);
// 					_delay_ms(200);
// 					while (sensorCount == 0 || weightedValue > 5) sensorValueConversion();
// 				}

				_delay_ms(150);
				sensorValueConversion();
				if (weightedValue > 2 || sensorCount == 0)
				{
// 					Backward();
// 					_delay_ms(350);
// 					sensorValueConversion();
// 					if(weightedValue > 2 || sensorCount == 0)
// 					{
// 						RightSharp(120);
// 						_delay_ms(400);
// 					}
					
					Backward();
					_delay_ms(350);
					sensorValueConversion();
					RightSharp(120);
					_delay_ms(400);
				}
				while (sensorCount == 0 || weightedValue > 5) sensorValueConversion();
			}
		}

		else if (allFlag == 1 && weightedValue < 7 && weightedValue > 3)
		{
// 			Stop();
// 			_delay_ms(2000);
			Forward();
			while (allFlag == 1 && stopFlag < 100) sensorValueConversion();

/*****************************************************Stop Sequence****************************************************/

			if (stopFlag >= 100) // work with stopflag
			{
				Backward();
				_delay_ms(100);
				Stop();
				_delay_ms(2000);
				while (sensorCount == 6 && allFlag == 1) sensorValueConversion();
			}
			else
			{
				_delay_ms(150);
				sensorValueConversion();
				RightSharp(160);
				if (sensorCount == 0 && leftFlag == 0)
				{
					Backward();
					_delay_ms(350);
					RightSharp(120);
					_delay_ms(200);
					while (sensorCount == 0 || weightedValue > 5) sensorValueConversion();
				}
			}
		}		

/**********************************************************PID********************************************************/

		if (weightedValue != 255)
		{
			PID_Correction();
			enable1(LMspeed);
			enable2(RMspeed);
		}
//  		sensorValuePrint();
	}
}


uint8_t medianFilter(uint8_t data[], uint8_t n)
{
	uint8_t i = 0, j = 0, key = 0;

	for(i = 1; i < n; i++)
	{
		key = data[i];
		j = i - 1;
		while(j >= 0 && data[j] > key)
		{
			data[j + 1] = data[j];
			j--;
		}
		data[j + 1] = key;
	}
	if (n % 2 == 1) return data[(n - 1) / 2];
	else return ((data[n / 2] + data[(n / 2) - 1]) / 2);
}

uint16_t sonar1Read(void)
{
	sonar1_ack = 0;
	trig1_PORT &= ~(1 << trig1_PIN);
	trig1_PORT |= (1 << trig1_PIN);
	_delay_us(10);
	trig1_PORT &= ~(1 << trig1_PIN);
	
	while (sonar1_ack == 0)
	{
		TCCR1B |= (1 << CS11);
		if ((PIND & (1 << PIND3)))
		{
			TCNT1 = 0;
			TCCR1B |= (1 << CS11);
			while ((PIND & (1 << PIND3)))
			{
				if (TCNT1 > 5000)
				{
					TCCR1B = 0;
					TCNT1 = 0;

					return 111;
				}
			}
			TCCR1B = 0;
			pulse1 = TCNT1 / 2;
			TCNT1 = 0;
			sonar1_ack = 1;
// 			Serial_sendInt(pulse1, DEC);
// 			Serial_sendString("\t");
		}
		else if (TCNT1 > 10000)
		{
			TCCR1B = 0;
			TCNT1 = 0;
			return 112;
		}

	}
	return (.035 * pulse1 / 2);
}

uint16_t sonar2Read(void)
{
	sonar2_ack = 0;
	trig2_PORT &= ~(1 << trig2_PIN);
	trig2_PORT |= (1 << trig2_PIN);
	_delay_us(10);
	trig2_PORT &= ~(1 << trig2_PIN);
	
	while (sonar2_ack == 0)
	{
		TCCR1B |= (1 << CS11);

		if ((PIND & (1 << PIND2)))
		{
			// 			TCCR1B |= (1 << CS11);
			TCNT1 = 0;
			while ((PIND & (1 << PIND2)))
			{
				if (TCNT1 > 5000)
				{
					TCCR1B = 0;
					TCNT1 = 0;
					return 111;
				}
			}
			TCCR1B = 0;
			pulse2 = TCNT1 / 2;
			TCNT1 = 0;
			sonar2_ack = 1;
// 			Serial_sendInt(pulse2, DEC);
// 			Serial_sendString("\t");
		}

		else if (TCNT1 > 10000)
		{
			TCCR1B = 0;
			TCNT1 = 0;
			return 112;
		}

	}
	return (.035 * pulse2 / 2);
}

void sensorValuePrint(void)
{
	int sensor = 0;
// 	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
// 	{
// 		Serial_sendInt(prev_SensorValue[sensorNumber], DEC);
// 		Serial_sendString("\t");
// 	}
// 	
	Serial_sendString("\t");
	Serial_sendInt(weightedValue, DEC);
	Serial_sendString("\t");
	Serial_sendInt(distance1, DEC);
 	Serial_sendString("\t");
// 	Serial_sendInt(leftFlag, DEC);
// 	Serial_sendString("\t");
// 	Serial_sendInt(rightFlag, DEC);
// 	Serial_sendString("\t");
// // 	Serial_sendInt(error, DEC);
// 	Serial_sendString("\t");
// 	Serial_sendInt(motorResponse, DEC);
// 	Serial_sendString("\t\t");
	
	Serial_sendInt(LMspeed, DEC);
	Serial_sendString("\t");
	Serial_sendInt(RMspeed, DEC);
	Serial_sendString("\t");
	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		sensor = adcRead(sensorNumber);
// 		if(sensor > SensorMaxValue[sensorNumber]) sensor = SensorMaxValue[sensorNumber];
// 		else if (sensor < SensorMinValue[sensorNumber]) sensor = SensorMinValue[sensorNumber];
// 		sensor = (sensor - SensorMinValue[sensorNumber]) * proportionality[sensorNumber]; 
		Serial_sendInt(sensor, DEC);
		Serial_sendString("\t");
	}
 	Serial_sendString("\n");
 	
}


void sensorValueConversion(void)
{
	uint8_t sum = 0;
	uint8_t maxSensor = 0;
	uint8_t minSensor = 5;
	sensorCount = 0;
	leftFlag = 0;
	rightFlag = 0;
	allFlag = 0;
	rightAcuteFlag = 0;
	leftAcuteFlag = 0;

	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		if(adcRead(sensorNumber) < threshold[sensorNumber])
		{
			if (sensorNumber == 0) leftFlag = 1;
			else if (sensorNumber == 7) rightFlag = 1;
			else
			{
				if ((sensorNumber - 1) < minSensor) minSensor = sensorNumber - 1;
				else if ((sensorNumber - 1) > maxSensor) maxSensor = sensorNumber - 1;
				sum += (sensorNumber - 1) * 2;
				sensorCount++;
			}
		}
	}
	
// 	if (sensorCount == (maxSensor - minSensor + 1))
// 	{
// 		if (maxSensor == 5 && minSensor > 1) 
// 		{
// 			rightAcuteFlag = 1;
// // 			Serial_sendString("R\n");
// 		}
// 		if (minSensor == 0 && maxSensor < 4) 
// 		{
// 			leftAcuteFlag = 1;
// // 			Serial_sendString("L\n");
// 		}
// 		if (rightAcuteFlag == 1 && leftAcuteFlag == 1)
// 		{
// 			rightAcuteFlag = 0; 
// 			leftAcuteFlag = 0;
// 		}
// 	}
	if (leftFlag == 1 && rightFlag == 1) 
	{
		leftFlag = 0;
		rightFlag = 0;
		allFlag = 1;
	}
	
	weightedValue = sum / sensorCount;
	
	if (sensorCount == 6 && allFlag == 1) stopFlag++;
	else stopFlag = 0;
}

void timercount(uint16_t durationmillis)
{
	TCCR1B |= (1 << WGM12);
	unsigned long timerlimit = ((F_CPU / 1024) * (durationmillis / 1000)) - 1;
	OCR1A = timerlimit;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void autoCalibration(void)
{
	int currentValue = 0;
	setTimer(2000);
	
	RightSharp(170);
	while (timerFlag == 0)
	{
		for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
		{
			currentValue = adcRead(sensorNumber);
			if(currentValue > SensorMaxValue[sensorNumber]) SensorMaxValue[sensorNumber] = currentValue;
			if(currentValue < SensorMinValue[sensorNumber]) SensorMinValue[sensorNumber] = currentValue;
		}
	}
	timerFlag = 0;
	timerOff();
	Stop();
	Serial_sendString("MAX\t");
	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		Serial_sendInt(SensorMaxValue[sensorNumber], DEC);
		Serial_sendChar('\t');
	}
	Serial_sendChar('\n');
	
	Serial_sendString("MIN\t");
	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		Serial_sendInt(SensorMinValue[sensorNumber], DEC);
		Serial_sendChar('\t');
	}
	Serial_sendChar('\n');
	
	Serial_sendString("THR\t");
	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		threshold[sensorNumber] = (SensorMaxValue[sensorNumber] + SensorMinValue[sensorNumber]) / 2;
		Serial_sendInt(threshold[sensorNumber], DEC);
		Serial_sendChar('\t');
	}
	Serial_sendChar('\n');
	
	Serial_sendString("UTH\t");
	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		upperThreshold[sensorNumber] = threshold[sensorNumber] + ((SensorMaxValue[sensorNumber] - threshold[sensorNumber]) * 30 / 100); 
		Serial_sendInt(upperThreshold[sensorNumber], DEC);
		Serial_sendChar('\t');
	}
	Serial_sendChar('\n');
	
	Serial_sendString("LTH\t");
	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		lowerThreshold[sensorNumber] = threshold[sensorNumber] - ((threshold[sensorNumber] - SensorMinValue[sensorNumber]) * 30 / 100);
		Serial_sendInt(lowerThreshold[sensorNumber], DEC);
		Serial_sendChar('\t');
	}
	Serial_sendChar('\n');
		
	Serial_sendString("PRO\t");
	for(uint8_t sensorNumber = 0; sensorNumber < 8; sensorNumber++)
	{
		proportionality[sensorNumber] = 20000 / (SensorMaxValue[sensorNumber] - SensorMinValue[sensorNumber]);
		Serial_sendInt(proportionality[sensorNumber], DEC);
		Serial_sendChar('\t');
	}
	Serial_sendString("\n\n\n\n\n\n");
	
	
}

void setTimer(uint16_t durationMicros)
{
	duration = durationMicros;
	TIMSK |= (1 << OCIE1A);										// Set Timer1 Interrupt ON
	sei();														// Set Global Interrupt
	TCNT1 = 0;													// Reset Timer Count
	OCR1A = 0xF9;												// Compare Value for 1ms
	TCCR1B |= (1 << CS10) | (1 << CS11) | (1 << WGM12);			// Set Prescaler to 64 and CTC Mode
}

void timerOff(void)
{
	TCCR1B = 0x00;
	TIMSK = 0x00;
	cli();
	TCNT1 = 0;
}

ISR(TIMER1_COMPA_vect)
{
	timerOverflow++;
	TCNT1 = 0;
	if (timerOverflow >= duration)
	{
		TCCR1B |= 0;
		timerFlag = 1;
		timerOverflow = 0;
		cli();
	}
}

void PID_Correction(void)
{
	error = weightedValue - setPoint;

	correction = ((Kp * error) + (Kd * (error - prev_error))/* + (Ki * sum_error)*/);

	prev_error = error;
	motorResponse = (int)correction;

	if(motorResponse > maxSpeed)
	{
		reverseSpeed = (motorResponse - maxSpeed);
		if (reverseSpeed > maxSpeed) reverseSpeed = maxSpeed;
		motorResponse = maxSpeed;
		
		setRotationRightSharp();
		
		RMspeed = reverseSpeed;
		LMspeed = maxSpeed/* - (maxSpeed / speedCoefficient)*/;
	}
	
	else if(motorResponse < -maxSpeed)
	{
		reverseSpeed = (-motorResponse - maxSpeed);
		if (reverseSpeed > maxSpeed) reverseSpeed = maxSpeed;
		motorResponse = -maxSpeed;
		
		setRotationLeftSharp();
		
		RMspeed = maxSpeed;
		LMspeed = reverseSpeed;
	}
	


	else if(motorResponse >= 0 && motorResponse < maxSpeed)
	{
		setRotationForward();
		
		RMspeed = maxSpeed - motorResponse/* - (maxSpeed / speedCoefficient)*/;
		LMspeed = maxSpeed;		
	}

	else if(motorResponse < 0 && motorResponse > -maxSpeed)
	{
		setRotationForward();
		
		RMspeed = maxSpeed/* - (maxSpeed / speedCoefficient)*/;
		LMspeed = maxSpeed + motorResponse;
	}
// 	setRotationForward();
// 	
// 	RMspeed = baseSpeed - motorResponse;
// 	LMspeed = baseSpeed + motorResponse;
// 	
// 	if (RMspeed < 0) RMspeed = 0;
// 	if (RMspeed > maxSpeed) RMspeed = maxSpeed;
// 	if (LMspeed < 0) LMspeed = 0;
// 	if (LMspeed > maxSpeed) LMspeed = maxSpeed;
// 
}

void Stop(void)
{
	Backward();
	_delay_us(100);
	enable1(0);
	enable2(0);
	
	motor1a_PORT &= ~(1 << motor1a_PIN);
	motor1b_PORT &= ~(1 << motor1b_PIN);
	motor2a_PORT &= ~(1 << motor2a_PIN);
	motor2b_PORT &= ~(1 << motor2b_PIN);
}

void setRotationForward(void)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT |= (1 << motor1a_PIN);
	motor1b_PORT &= ~(1 << motor1b_PIN);
	motor2a_PORT |= (1 << motor2a_PIN);
	motor2b_PORT &= ~(1 << motor2b_PIN);
}

void setRotationLeftSharp(void)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT &= ~(1 << motor1a_PIN);
	motor1b_PORT |= (1 << motor1b_PIN);
	motor2a_PORT |= (1 << motor2a_PIN);
	motor2b_PORT &= ~(1 << motor2b_PIN);
}

void setRotationRightSharp(void)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT |= (1 << motor1a_PIN);
	motor1b_PORT &= ~(1 << motor1b_PIN);
	motor2a_PORT &= ~(1 << motor2a_PIN);
	motor2b_PORT |= (1 << motor2b_PIN);
}

void Forward(void)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT |= (1 << motor1a_PIN);
	motor1b_PORT &= ~(1 << motor1b_PIN);
	motor2a_PORT |= (1 << motor2a_PIN);
	motor2b_PORT &= ~(1 << motor2b_PIN);

	enable1(maxSpeed);
	enable2(maxSpeed);
}

void Forward_slow(void)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT |= (1 << motor1a_PIN);
	motor1b_PORT &= ~(1 << motor1b_PIN);
	motor2a_PORT |= (1 << motor2a_PIN);
	motor2b_PORT &= ~(1 << motor2b_PIN);

	enable1(140);
	enable2(140);
}


void Backward(void)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT &= ~(1 << motor1a_PIN);
	motor1b_PORT |= (1 << motor1b_PIN);
	motor2a_PORT &= ~(1 << motor2a_PIN);
	motor2b_PORT |= (1 << motor2b_PIN);

	enable1(130);
	enable2(130);
}

void RightSharp(uint8_t motorSpeed)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT |= (1 << motor1a_PIN);
	motor1b_PORT &= ~(1 << motor1b_PIN);
	motor2a_PORT &= ~(1 << motor2a_PIN);
	motor2b_PORT |= (1 << motor2b_PIN);

	enable1(motorSpeed);
	enable2(motorSpeed);
}

void LeftSharp(uint8_t motorSpeed)
{
	enable1(0);
	enable2(0);
	
	motor1a_PORT &= ~(1 << motor1a_PIN);
	motor1b_PORT |= (1 << motor1b_PIN);
	motor2a_PORT |= (1 << motor2a_PIN);
	motor2b_PORT &= ~(1 << motor2b_PIN);

	enable1(motorSpeed);
	enable2(motorSpeed);
}





