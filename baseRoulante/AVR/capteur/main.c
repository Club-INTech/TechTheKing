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
    
//     volatile uint16_t temp1;
//     volatile uint16_t temp2;
    
    while(1) {
        _delay_ms(50);
        ultrason = ping(PIN_ULTRASON1);
//         ultrason = temp1?(temp1<temp2):temp2;
//         _delay_ms(50);
//         temp2 = ping(PIN_ULTRASON2);
//         ultrason = temp2?(temp2<temp1):temp1;
    }
    
    return 0;
}

