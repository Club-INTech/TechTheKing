#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "twi_slave.h"
#include "compteur.h"



int main( void )
{

	uart_init();
	
    // Interruptions
    sei();

    // I2C
    TWI_Init();

    // Compteur
    compteur_init();


    
    while(1) {
			printlnLong(roue1);
	printlnLong(roue2);
        TWI_Loop();
    }

    return 0;
    
}
