#include <util/delay.h>
#include "actionneurs.h"
#include "twi_slave.h"
#include "serial.h"

int main()
{
    init();
    
    while(1)
    {
        asservissement();
    }
    
    return 0;
}
