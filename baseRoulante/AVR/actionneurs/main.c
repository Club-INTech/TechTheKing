#include <util/delay.h>
#include "actionneurs.h"
#include "twi_slave.h"
#include "serial.h"

int main()
{
    init();
    uart_init();
    
    while(1)
    {
    }
    
    return 0;
}
