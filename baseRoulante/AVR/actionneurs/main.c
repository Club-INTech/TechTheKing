#include <util/delay.h>
#include "actionneurs.h"
#include "ax12.h"
#include "twi_slave.h"
#include "adc.h"
//#include "serial.h"

int main()
{
    // Pour le debug serie
    //uart_init();

    // Initialisation de la freq de la serie pour AX12
    writeData (0XFE, 0X04, 1, 0X09);

    // Initialisations diverses (et variees)
    actio_init();

    // Initialisation I2C
    TWI_Init();

    //Initialisation de l'adc
    unsigned char conf = 0;
    conf |= ( 1 << ADC_REF0 ) | ( 1 << ADC_CHOOSE ) | ADC1;
    conf &= ~( 1 << ADC_REF1 );
    adcInit(conf);

    // Initialisation pour l'AX12 gauche
    AX12Init (AX_ID1, AX_ANGLE_EXT1, AX_ANGLE_EXT2, AX_SPEED);
    // Initialisation pour l'AX12 droite
    AX12Init (AX_ID2, AX_ANGLE_EXT1, AX_ANGLE_EXT2, AX_SPEED);

    // Position initiale aimants
    SERVO_CONS1 = SERVO_PWM_UP1;
    SERVO_CONS2 = SERVO_PWM_UP2;

    //recalage();
    
    while(1)
    {
        /*************** Asservissement ***************/
        if (etat_asservissement == ASC_ASSERV_SYNCHRO)
            asservissement_synchro();
        else if (etat_asservissement == ASC_ASSERV_INDEP)
            asservissement();
        /**********************************************/

    //          /*** Pour les tests ***/
    //          _delay_ms(2000);
    //          AX12GoTo(AX_ID2, 511);
    //          _delay_ms(2000);
    //          AX12GoTo(AX_ID2, 200);
    //          _delay_ms(2000);
    //          AX12GoTo(AX_ID2,800);
    
    //         _delay_ms(2000);
    //         SERVO_CONS1 = SERVO_PWM_UP1;
    //         _delay_ms(2000);
    //         SERVO_CONS1 = SERVO_PWM_DOWN1;
    
    //         _delay_ms(250);
    //         printLong(ascenseur1);
    //         printString(" ; ");
    //         printlnLong(ascenseur2);
    }

    return 0;
}
