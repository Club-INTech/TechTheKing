#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../common/usart.h"

int main( void )
{
	uart_init();
	_delay_ms(1000);
	printlnString("## Debut de test unitaire ##");
	println();
	println();
	
	while( 42 )
	{
		_delay_ms(1000);
	}
	
	return 0;
}