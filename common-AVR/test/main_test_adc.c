#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../usart.h"
#include "../adc.h"

int main()
{
	uart_init();
	_delay_ms(1000);
	unsigned char conf = 0;
	conf |= ( 1 << ADC_REF0 ) | ( 1 << ADC_CHOOSE ) | ADC1;
	conf &= ~( 1 << ADC_REF1 );
	adcInit(conf);
	printlnString("Depart");
	while( 1 )
	{
		printlnULong((unsigned long)adcRead(ADC5));
		_delay_ms(200);
	}
	return 0;
}