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
    
    while(1) {
        _delay_ms(25);
        
        uint32_t temp1 = 0;
        uint32_t temp2 = 0;
        
        uint8_t i;
        for (i = 0; i < 2 ; i++) {
            // Polling ultrason
            temp1 += ping(PIN_ULTRASON1);
            temp2 += ping(PIN_ULTRASON2);
        }
        
        ultrason = (temp1 + temp2) >> 2;
    }

    return 0;
}
 
