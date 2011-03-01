#include <util/delay.h>
#include "actionneurs.h"
#include "i2c.h"

uint8_t hello[] = {'h', 'e', 'l', 'l', 'o', ' '};
void onRequest ();

int main()
{
    sei();
    
    i2c_beginSlave(2);
    i2c_onRequest( onRequest );

    while(1)
    {
    }
    
    return 0;
}

void onRequest ()
{
    i2c_sendArray(hello, 6);
}