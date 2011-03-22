#include "serial.h"

/*
 * Partie réception de DATA
 */

struct ring_buffer rx_buffer = { { 0 }, 0, 0 };

ISR(USART_RX_vect)
{
	unsigned char c = UDR0;
	store_char(c, &rx_buffer);
}

inline void store_char(unsigned char c, struct ring_buffer *rx_buffer)
{
	int16_t i = (rx_buffer->head + 1) % RX_BUFFER_SIZE;
	
	if (i != rx_buffer->tail)
	{
		rx_buffer->buffer[rx_buffer->head] = c;
		rx_buffer->head = i;
	}
}

uint8_t available(void)
{
	return (RX_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % RX_BUFFER_SIZE;
}

int16_t read(void)
{
	if (rx_buffer.head == rx_buffer.tail)
	{
		return -1;
	}
	else
	{
		unsigned char c = rx_buffer.buffer[rx_buffer.tail];
		rx_buffer.tail = (rx_buffer.tail + 1) % RX_BUFFER_SIZE;
		return c;
	}
}


/*
 * Définition des fonctions de bases pour le fonctionnement de la liaison série
 */

unsigned char uart_recv_char(void)
{
	while ( ! (UCSR0A & (1 << RXC0)) );
	
	return UDR0;
}

inline void uart_send_ln(void )
{
	uart_send_char('\r');
	uart_send_char('\n');
}

inline void uart_send_char(unsigned char byte)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = byte;
}

inline void uart_send_string(const char *string)
{
	unsigned char i;
	for (i = 0 ; string[i] != '\0' ; i++)
		uart_send_char(string[i]);
}

void uart_init( void )
{
	UBRR0H = (unsigned char)(UBRR >> 8);
	UBRR0L = (unsigned char)UBRR;
	
	UCSR0B |= ( 1 << RXCIE0 );	//Activation de l'interruption de réception
	
	
	UCSR0B |= ( 1 << RXEN0 );	//Activation de la réception
	UCSR0B |= ( 1 << TXEN0 );	//Activation de l'emission

	UCSR0C = (1 << USBS0)|(3<<UCSZ00);
	sei();
}

/*
 * Définition des foncions printnumber
 */

inline void printIntNumber(uint16_t n)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	uint16_t i = 0;

	if (n == 0)
	{
		uart_send_char('0');
		return;
	} 

	while (n > 0)
	{
		buf[i++] = n % 10;
		n /= 10;
	}

	for (; i > 0; i--)
		uart_send_char((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}

inline void printShortNumber(unsigned short n)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	uint16_t i = 0;

	if (n == 0)
	{
		uart_send_char('0');
		return;
	} 

	while (n > 0)
	{
		buf[i++] = n % 10;
		n /= 10;
	}

	for (; i > 0; i--)
		uart_send_char((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}

inline void printLongNumber(unsigned long n )
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	uint16_t i = 0;

	if (n == 0)
	{
		uart_send_char('0');
		return;
	} 

	while (n > 0)
	{
		buf[i++] = n % 10;
		n /= 10;
	}

	for (; i > 0; i--)
		uart_send_char((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}

/*
 * Définition des print16_t pour nombre
 */

void printString(const char *string )
{
	uart_send_string(string);
}

void printChar( unsigned char c )
{
	uart_send_char( c );
}

void println( void )
{
	uart_send_ln();
}

void printlnChar( unsigned char c)
{
	uart_send_char( c );
	uart_send_ln();
}

void printlnString(const char *string )
{
	uart_send_string(string);
	uart_send_ln();
}

void printUInt(uint16_t entier )
{
	printIntNumber(entier);
}

void printUShort(uint8_t entier )
{
	printShortNumber( entier );
}

void printULong(uint32_t entier )
{
	printLongNumber(entier);
}

void printlnUShort(uint8_t entier)
{
	printShortNumber(entier);
	uart_send_ln();
}

void printlnUInt(uint16_t entier)
{
	printIntNumber(entier);
	uart_send_ln();
}

void printlnULong(uint32_t entier)
{
	printLongNumber(entier);
	uart_send_ln();
}

void printShort(int8_t entier)
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	printShortNumber(entier);
}

void printInt(int16_t entier)
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	printIntNumber(entier);
}

void printLong(int32_t entier)
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	printLongNumber(entier);
}

void printlnShort(int8_t entier )
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	printShortNumber(entier);
	uart_send_ln();
}

void printlnInt(int16_t entier )
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	printIntNumber(entier);
	uart_send_ln();
}

void printlnLong(int32_t entier )
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	printLongNumber(entier);
	uart_send_ln();
}

inline long readLongNumber( void )
{
	long monLong = 0; 
	uint16_t i = 0;
	uint8_t flag = 0;
	
	while( 1 )
	{
		if( available() )
		{
			i = read();
			if( i == '-' )
			{
				flag = 1;
			}
			else if( i == '\r' || i == '\n' || i == 0b00001010 || i == '@')
			{
				if( flag == 1 )
					return -1 * monLong;
				else
					return monLong;
			}
			else if( i <= '9' && i >= '0' )
			{
				monLong *= 10;
				monLong += (i - 48);
			}
		}
	}
}

inline uint32_t readULongNumber( void )
{
	uint32_t monLong = 0; 
	uint16_t i = 0;
	
	while( 1 )
	{
		if( available() )
		{
			i = read();
			if( i == '\r' || i == '\n' || i == 0b00001010 )
			{
				return monLong;
			}
			else if( i <= '9' && i >= '0' )
			{
				monLong *= (unsigned short)10;
				monLong += (unsigned short)(i - 48);
			}
		}
	}
}
