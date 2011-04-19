#include "serial.h"
#include "ultrasons.h"

int main()
{
    uart_init();
    
    while(1)
    {
        printlnLong(ping(PIN_ULTRASON_1));
        _delay_ms(250);
    }

    return 0;
}
 
