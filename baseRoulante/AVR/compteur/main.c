#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "twi_slave.h"
#include "compteur.h"

int main( void ){
	
    // Interruptions
    sei();
    
    //Série
    //uart_init();
    
    // I2C
    TWI_Init();

    // Compteur
    compteur_init();
    
    while(1) {
		//printlnLong(roue1);
        TWI_Loop();
    }

    return 0;
    
}
