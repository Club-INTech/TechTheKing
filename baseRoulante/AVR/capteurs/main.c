#include "serial.h"

int main()
{
    uart_init();
    while(1)
    {
        printlnLong(42);
    }

    return 0;
}
 
