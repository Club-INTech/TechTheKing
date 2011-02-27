#include<util/delay.h>
#include "actionneurs.h"
#include "i2c.h"

int main()
{
    init();

    uint8_t sense = 1;
    
    while (1)
    {
        if (sense == 1)
            SERVO1++;
        else
            SERVO1--;

        if (SERVO1 == 0)
            sense == 1;
        if (SERVO1 == 255)
            sense == 0;

        _delay_ms(1);
    }
    
    return 0;
}
