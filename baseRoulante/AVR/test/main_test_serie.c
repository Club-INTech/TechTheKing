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
	uint8_t i = 42, j = 1 ,c ;
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
	printlnString("Appuyez sur @ pour sortir et faire le test suivant");
	
	while( j )
	{
		if( available() )
		{
			c = read();
			uart_send_char(c);
			if( c == '@')
				j = 0;
		}
	}
	
	j = 1;
	long monNombre = 0;
	println();
	printlnString("Cette fois-ci, veuillez m'envoyer un nombre avec 'ENTER' à la fin");
	printlnString("Vous avez droit à 3 essais (il y a une seconde de latence entre le second et le troisième, mais vou spouvez quand même entrer un nombre avant");
	printlnString("Le nombre doit-être compris entre -2147483646 et 2147483647");
	
	monNombre = readLongNumber();
	printlnLong(monNombre);
	monNombre = readLongNumber();
	printlnLong(monNombre);
	_delay_ms(1000);
	monNombre = readLongNumber();
	printlnLong(monNombre);
	
	println();
	printlnString("Nous recommençons la même manipulation, mais pour un nombre non signé compris entre 0 et 2^32");
	
	monNombre = readULongNumber();
	printlnLong(monNombre);
	monNombre = readULongNumber();
	printlnLong(monNombre);
	_delay_ms(1000);
	monNombre = readULongNumber();
	printlnLong(monNombre);
	
	return 0;
}