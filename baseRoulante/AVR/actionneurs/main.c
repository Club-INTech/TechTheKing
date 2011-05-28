#include <util/delay.h>
#include "actionneurs.h"
#include "ax12.h"
#include "twi_slave.h"
#include "adc.h"
//#include "serial.h"

int main()
{
<<<<<<< HEAD
	// Pour le debug serie
	//uart_init();

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

	SERVO_CONS1 = SERVO_PWM_UP1;
	SERVO_CONS2 = SERVO_PWM_UP2;

	while(1)
	{
		/*************** Asservissement ***************/
		if (etat_asservissement == ASC_ASSERV_SYNCHRO)
			asservissement_synchro();
		else if (etat_asservissement == ASC_ASSERV_INDEP)
			asservissement();
		/**********************************************/

		/********************* I2C ********************/
		// Je l'ai dans l'interruption TWI
		// TWI_Loop();
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

	//         _delay_ms(250);
	//         printLong(ascenseur1);
	//         printString(" ; ");
	//         printlnLong(ascenseur2);
	}

	return 0;
=======
    // Pour le debug serie
    //uart_init();
    
    // Initialisations diverses (et variees)
    actio_init();
    
    // Initialisation I2C
    TWI_Init();
    
    // Initialisation pour l'AX12 gauche
    AX12Init (AX_ID1, AX_ANGLE_EXT1, AX_ANGLE_EXT2, AX_SPEED);
    // Initialisation pour l'AX12 droite
    AX12Init (AX_ID2, AX_ANGLE_EXT1, AX_ANGLE_EXT2, AX_SPEED);
    
    SERVO_CONS1 = SERVO_PWM_UP1;
    SERVO_CONS2 = SERVO_PWM_UP2;
    
	consigne1 = 5000;

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

//         _delay_ms(250);
//         printLong(ascenseur1);
//         printString(" ; ");
//         printlnLong(ascenseur2);
    }
    
    return 0;
>>>>>>> 98c14071f3c7daef071740fd44cd979deb86c13e
}
