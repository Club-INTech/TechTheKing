#ifndef _USART_H_
#define _USART_H_

void uart_init(void);
void print(const char *);
void println( void );
unsigned char uart_recv_char(void);
void uart_send_char(unsigned char);

void print_ushort( uint8_t entier );
void print_uint( uint16_t entier );
void print_ulong( uint32_t entier );

void print_short( int8_t entier );
void print_int( int16_t entier );
void print_long( int32_t entier );

#define BAUD_RATE 57600

#define UBRR (F_CPU/8/BAUD_RATE - 1)/2

#endif
