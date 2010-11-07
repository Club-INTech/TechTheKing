#ifndef _USART_H_
#define _USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_RATE 57600

#define UBRR (F_CPU/8/BAUD_RATE - 1)/2

//Fonction de base de la liaison série
void uart_init(void);
unsigned char uart_recv_char(void);
inline void uart_send_char(unsigned char);
inline void uart_send_ln( void );
inline void uart_send_string(const char *);

//Fonction suplémentaire
inline void printShortNumber( unsigned short );
inline void printIntNumber( unsigned int );
inline void printLongNumber( unsigned long );

//Définition des prints
void printString( const char * );
void printShort( short );
void printUShort( unsigned short );
void printInt( int );
void printUInt( unsigned int );
void printLong( long );
void printULong( unsigned long );


//Définition des prints ln
void println( void );
void printlnString( const char * );
//void printlnShort( short );
void printlnUShort( unsigned short );
//void printlnInt( int );
void printlnUInt( unsigned int );
// void printlnLong( long );
void printlnULong( unsigned long );

#endif
