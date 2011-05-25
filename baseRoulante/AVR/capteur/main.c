#include "serial.h"
#include "ultrasons.h"
#include "twi_slave.h"
#include <util/delay.h>

int main()
{
    TWI_Init();
    
    
    while(1) {

       _delay_ms(100);
       
       ultra1 = ping(PIN_ULTRASON_1);
       ultra2 = ping(PIN_ULTRASON_2);
       ultra3 = ping(PIN_ULTRASON_3);
       
    }

    return 0;
}
 
