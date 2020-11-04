#include "serial_lib.h"

void UART_init(void)
{
	UCSRC &= ~(1 << UMSEL);						// Asynchronous Mode Selection
	UCSRA &= ~(1 << U2X);						// Normal Transmission Mode
	UBRRH = 0;
	UBRRL = 103;
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}

// void serial.sendInt(unsigned int data)
// {
// 	while (!(UCSRA & (1 << UDRE)));
// 	
// 	UCSRB &= ~(1 << TXB8);
// 	
// 	if (data & 0x0100) UCSRB |= (1 << TXB8);
// 	
// 	UDR = data;
// }

void Serial_sendInt(unsigned int data, int intBase)
{
	while (!(UCSRA & (1 << UDRE)));
	char intBuffer[20];
	itoa(data, intBuffer, intBase);
	Serial_sendString(intBuffer);
}

void Serial_sendChar(char c)
{
	while (!(UCSRA & (1 << UDRE)));
	UDR = c;
}

void Serial_sendString(char *s)
{
	for(int i = 0; s[i] != '\0'; i++)
	{
		Serial_sendChar(s[i]);
	}
}


unsigned char Serial_receive(void)
{
	while (!(UCSRA & (1 << RXC)));
	return UDR;
}