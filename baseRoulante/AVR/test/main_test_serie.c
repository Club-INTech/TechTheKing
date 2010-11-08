/**
 * @author Jérémy Cheynet
 * @brief Test unitaire de la lib usart
 * @version 1.0
 * @date 07/11/2010
 * 
 */
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
	i++;
	printString("Impression d'un unsigned int 42 : ");
	printUInt((unsigned int)i);
	println();
	i++;
	printString("Impression d'un unsigned long 42 : ");
	printULong((unsigned long)i);
	println();
	println();
	i++;
	
	printlnString("### Debut de zone de test des nombres unsigned avec le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printlnUShort(i);
	i++;
	printString("Impression d'un unsigned int 42 : ");
	printlnUInt((unsigned int)i);
	i++;
	printString("Impression d'un unsigned long 42 : ");
	printlnULong((unsigned long)i);
	i++;
	println();
	
	printlnString("### Debut de zone de test des nombres signed (mais positif) sans le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printShort((short)i);
	i++;
	println();
	printString("Impression d'un unsigned int 42 : ");
	printInt((int)i);
	i++;
	println();
	printString("Impression d'un unsigned long 42 : ");
	printLong((long)i);
	i++;
	println();
	println();
	
	printlnString("### Debut de zone de test des nombres signed (mais negatif) sans le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printShort(-i);
	i++;
	println();
	printString("Impression d'un unsigned int 42 : ");
	printInt((int)-i);
	i++;
	println();
	printString("Impression d'un unsigned long 42 : ");
	printLong((long)-i);
	i++;
	println();
	println();
	
	printlnString("### Debut de zone de test des nombres signed (mais negatif) avec le retour a la ligne automatique (ln) ###");
	println();
	printString("Impression d'un unsigned short 42 : ");
	printlnShort(-i);
	i++;
	printString("Impression d'un unsigned int 42 : ");
	printlnInt((int)-i);
	i++;
	printString("Impression d'un unsigned long 42 : ");
	printlnLong((long)-i);
	i++;
	println();
	println();
	
	printlnString("Test de réceptio par le PC : ecrivez du texte, je vais vous repondre.");
	
	while( 42 )
	{
		if( available() )
			uart_send_char(read()+1);
	}
	
	return 0;
}