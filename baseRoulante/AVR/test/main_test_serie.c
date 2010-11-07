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
	
	printlnString("### Debut de zone de test des nombres unsigned sans le retour a la ligne automatique (ln) ###");
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
	println();
	
	printlnString("### Debut de zone de test des nombres unsigned avec le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printlnUShort(i);
	printString("Impression d'un unsigned int 42 : ");
	printlnUInt((unsigned int)i);
	printString("Impression d'un unsigned long 42 : ");
	printlnULong((unsigned long)i);
	println();
	
	printlnString("### Debut de zone de test des nombres signed (mais positif) sans le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printShort((short)i);
	println();
	printString("Impression d'un unsigned int 42 : ");
	printInt((int)i);
	println();
	printString("Impression d'un unsigned long 42 : ");
	printLong((long)i);
	println();
	println();
	
	printlnString("### Debut de zone de test des nombres signed (mais négatif) sans le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printShort(-i);
	println();
	printString("Impression d'un unsigned int 42 : ");
	printInt((int)-i);
	println();
	printString("Impression d'un unsigned long 42 : ");
	printLong((long)-i);
	println();
	println();
	
	printlnString("### Debut de zone de test des nombres signed (mais négatif) avec le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printlnShort(-i);
	printString("Impression d'un unsigned int 42 : ");
	printlnInt((int)-i);
	printString("Impression d'un unsigned long 42 : ");
	printlnLong((long)-i);
	println();
	println();
	
	
	while( 42 )
	{
		_delay_ms(1000);
	}
	
	return 0;
}