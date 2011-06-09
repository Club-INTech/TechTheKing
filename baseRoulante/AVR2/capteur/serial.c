#include "serial.h"

volatile unsigned char lcb_val = 'P';

/*
 * Partie réception de DATA
 */

struct ring_buffer rx_buffer = { { 0 }, 0, 0 };

ISR(USART_RX_vect)
{
	unsigned char c = UDR0;
    
    if (c == 'Q') 
        lcb_val = 'Q';
   
    else if (c == 'K')
        lcb_val = 'K';
}

inline void store_char(unsigned char c, struct ring_buffer *rx_buffer)
{
	int i = (rx_buffer->head + 1) % RX_BUFFER_SIZE;
	
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

int read(void)
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

// Utiles en cas de debug

inline void printShortNumber(unsigned short n)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned int i = 0;

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

void printlnUShort(short unsigned int entier)
{
	printShortNumber(entier);
	uart_send_ln();
}

void printlnShort(short int entier )
{
	if (entier < 0)
	{
		uart_send_char('-');
		entier = -entier;
	}
	printShortNumber(entier);
	uart_send_ln();
}
