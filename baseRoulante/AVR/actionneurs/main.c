#include <util/delay.h>
#include "actionneurs.h"
#include "serial.h"
#include "twi_slave.h"

int main()
{
    init();
    TWI_Init();
    
    AX12Init (1, 0, 1023, 511);
    
    
    while(1)
    {
        /*************** Asservissement ***************/
        if (etat_asservissement == ASSERV_SYNCHRO)
            asservissement_synchro();
        else if (etat_asservissement == ASSERV_INDEP)
            asservissement();
        /**********************************************/
        
        /********************* I2C ********************/
        TWI_Loop();
        /**********************************************/

//     /** Pour les tests **/
//     _delay_ms(2000);
//     SERVO2 = PWM_UP;
//     AX12GoTo(1, 400);
//     _delay_ms(2000);
//     SERVO2 = PWM_DOWN;
//     AX12GoTo(1, 600);
    }
    
    return 0;
}
