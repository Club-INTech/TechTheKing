#include <util/delay.h>

#include "compteur.h"

int main()
{
    init();
    
    roue1 = 0;
    roue2 = 0;

    i2c_onRequest(buffer_send);
    
    while(1)
    {
        _delay_ms(10);
    }
    
    return 0;
}

