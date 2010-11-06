#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

int main( void )
{
	uart_init();

	while( 1 )
	{
		_delay_ms(1000);
		uart_send_char('@');
		println();
	}
	return 1;
}