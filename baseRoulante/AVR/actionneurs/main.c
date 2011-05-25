#include <util/delay.h>
#include "actionneurs.h"
#include "serial.h"
#include "twi_slave.h"

int main()
{
    init();
    TWI_Init();
    
    etat_asservissement = ASSERV_SYNCHRO;
    
    while(1)
    {
        /*************** Asservissement ***************/
        if (etat_asservissement == ASSERV_SYNCHRO)
            asservissement_synchro();
        else if (etat_asservissement == ASSERV_INDEP)
            asservissement();
        /**********************************************/
    }
    
    return 0;
}
