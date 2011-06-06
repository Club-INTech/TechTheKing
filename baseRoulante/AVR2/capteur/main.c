#include <util/delay.h>

#include "serial.h"
#include "capteurs.h"
#include "twi_slave.h"

int main()
{
    // Initialisation I2C
    TWI_Init();
    
    // Initialisation serie
    uart_init();
    
    volatile uint16_t temp1 = ping(PIN_ULTRASON1);
    _delay_ms(50);
    volatile uint16_t temp2 = ping(PIN_ULTRASON2);
    
    while(1) {
        
        _delay_ms(15);
        if ( temp2 < temp1 )
            ultrason = temp2;
        temp1 = ping(PIN_ULTRASON1);
        
        _delay_ms(15);
        if ( temp1 < temp2 )
            ultrason = temp1;
        temp2 = ping(PIN_ULTRASON2);
        
    }
    
    return 0;
}

