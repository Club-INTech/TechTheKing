#include <util/delay.h>
#include "actionneurs.h"
#include "twi_slave.h"

int main()
{
    // Initialisations diverses (et variees)
    init();
    
    // Initialisation I2C
    TWI_Init();
    
    // Initialisation pour l'AX12 gauche
    AX12Init (AX_ID1, AX_ANGLE_EXT1, AX_ANGLE_EXT2, AX_SPEED);
    // Initialisation pour l'AX12 droite
    //AX12Init (AX_ID2, AX_ANGLE_EXT1, AX_ANGLE_EXT2, AX_SPEED);
    
    while(1)
    {
        /*************** Asservissement ***************/
        if (etat_asservissement == ASC_ASSERV_SYNCHRO)
            asservissement_synchro();
        else if (etat_asservissement == ASC_ASSERV_INDEP)
            asservissement();
        /**********************************************/
        
        /********************* I2C ********************/
        TWI_Loop();
        /**********************************************/

//         /*** Pour les tests ***/
//         _delay_ms(2000);
//         //SERVO2 = PWM_UP;
//         AX12GoTo(0XFE, 511);
//         _delay_ms(2000);
//         //SERVO2 = PWM_DOWN;
//         AX12GoTo(0XFE, 200);
//         _delay_ms(2000);
//         AX12GoTo(0XFE,800);
    }
    
    return 0;
}
