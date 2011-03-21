#include "asservissement.h"

int main()
{
    init();
    
    while(1)
    {
        printLong(angle());
        _delay_ms(1000);
        printLong(distance());
        _delay_ms(1000);
    }
    
    return 0;
}