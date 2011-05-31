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
        _delay_ms(50);
        uint16_t temp1 = ping(PIN_ULTRASON1);
        _delay_ms(50);
        uint16_t temp2 = ping(PIN_ULTRASON2);
        
        ultrason = (temp1 <= temp2)?temp1:temp2;
    }

    return 0;
}
 
