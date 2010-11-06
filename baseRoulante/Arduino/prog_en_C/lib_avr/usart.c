#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

void print(const char *string)
{
	unsigned char i;
	for (i = 0 ; string[i] != '\0' ; i++)
		uart_send_char(string[i]);
}

unsigned char uart_recv_char(void)
{
	while ( ! (UCSR0A & (1 << RXC0)) );
	
	return UDR0;
}

void print_uint(uint16_t entier)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned long i = 0;

	if (entier == 0)
	{
		uart_send_char('0');
		return;
	}

	while (entier > 0)
	{
		buf[i++] = entier % 10;
		entier /= 10;
	}

	for (; i > 0; i--)
		uart_send_char((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] :'A' + buf[i - 1] - 10));
}

void print_ulong(uint32_t entier)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned long i = 0;

	if (entier == 0)
	{
		uart_send_char('0');
		return;
	}

	while (entier > 0)
	{
		buf[i++] = entier % 10;
		entier /= 10;
	}

	for (; i > 0; i--)
		uart_send_char((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] :'A' + buf[i - 1] - 10));
}

void print_ushort(uint8_t entier)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned long i = 0;

	if (entier == 0)
	{
		uart_send_char('0');
		return;
	}

	while (entier > 0)
	{
		buf[i++] = entier % 10;
		entier /= 10;
	}

	for (; i > 0; i--)
		uart_send_char((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] :'A' + buf[i - 1] - 10));
}

void print_int(int16_t entier)
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	print_int((uint16_t)entier);
}

void print_long(int32_t entier)
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	print_int((uint32_t)entier);
}

void print_short(int8_t entier)
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	print_int((uint8_t)entier);
}

void println(void )
{
	uart_send_char('\r');
	uart_send_char('\n');
}



void uart_send_char(unsigned char byte)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = byte;
}

void uart_init(void)
{
	UBRR0H = (unsigned char)(UBRR >> 8);
	UBRR0L = (unsigned char)UBRR;
	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	UCSR0C = (1 << USBS0)|(3<<UCSZ00);
}
