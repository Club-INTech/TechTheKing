#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../common/usart.h"

int main( void )
{
	uint8_t i = 42 ;
	uart_init();
	_delay_ms(1000);
	printlnString("## Debut de test unitaire ##");
	println();
	println();
	
	printlnString("### Debut de zone de test des nombres unsigned ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printUShort(i);
	println();
	printString("Impression d'un unsigned int 42 : ");
	printUInt((unsigned int)i);
	println();
	printString("Impression d'un unsigned long 42 : ");
	printULong((unsigned long)i);
	println();
	
	
	while( 42 )
	{
		_delay_ms(1000);
	}
	
	return 0;
}