#include <util/delay.h>
#include "actionneurs.h"
#include "twi_slave.h"

int main()
{
    init();
    TWI_Init();
    
    while(1)
    {
        TWI_Loop();
    }
    
    return 0;
}
