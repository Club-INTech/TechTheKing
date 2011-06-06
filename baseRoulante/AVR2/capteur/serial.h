#ifndef _USART_H_
#define _USART_H_

// Librairie C avr pour atmega 168 et 328 pour l'utilisation de liaison serie 
// Jérémy Cheynet avec le support psychologique de Yann Sionneau

// Purgee par la suite pour une utilisation plus specifique

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile unsigned char lcb_val;

#define BAUD_RATE 19200

#define UBRR (F_CPU/8/BAUD_RATE - 1)/2

#define RX_BUFFER_SIZE 32

struct ring_buffer
{
	unsigned char buffer[RX_BUFFER_SIZE];
	int head;
	int tail;
};

void uart_init(void);

unsigned char uart_recv_char(void);

inline void uart_send_char(unsigned char);

inline void uart_send_ln( void );

inline void printShortNumber( unsigned short );

void printlnShort( short );

void printlnUShort( unsigned short );

uint8_t available(void);

inline void store_char(unsigned char, struct ring_buffer *);

int read(void);

#endif
