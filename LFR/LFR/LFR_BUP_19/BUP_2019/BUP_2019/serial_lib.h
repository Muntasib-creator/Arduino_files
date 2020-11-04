#ifndef serial_lib

#define serial_lib

#define DEC				10
#define BIN				2
#define OCT				8
#define HEX				16

#include <avr/io.h>
#include <stdlib.h>

void UART_init(void);

void Serial_sendInt(unsigned int data, int intBase);

void Serial_sendChar(char c);

void Serial_sendString(char *s);

unsigned char Serial_receive(void);

#endif