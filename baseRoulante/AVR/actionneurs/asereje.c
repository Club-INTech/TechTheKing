#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define PULSATION 150
#define CHANGEMENT 150
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
    AX12Init (AX_ID1, 211, 811, AX_SPEED);
    // Initialisation pour l'AX12 droite
    AX12Init (AX_ID2, 211, 811, AX_SPEED);

    // Position initiale aimants
    SERVO_CONS1 = SERVO_PWM_UP1;
    SERVO_CONS2 = SERVO_PWM_UP2;

    	TCCR1B &= ~( 1 << CS12 );
	TCCR1B &= ~( 1 << CS11 );
	TCCR1B |= ( 1 << CS10 );
	//Mise en mode normal du timer 1
	TCCR1B &= ~( 1 << WGM13 );
	TCCR1B &= ~( 1 << WGM12 );
	TCCR1A &= ~( 1 << WGM11 );
	TCCR1A &= ~( 1 << WGM10 );
	//Activation global des interruptions
	sei();
	//Mise à zéro de la valeur du TIMER1 pour en pas avoir de problème lors de l'activation de celui-ci
	TCNT1H = 0;
	TCNT1L = 0;
	//Activation de l'interruption d'overflow du TIMER1
	TIMSK1 |= ( 1 << TOIE1 );

	consigne1 = ASC_CONS_MED;
	consigne2 = ASC_CONS_MAX;

	_delay_ms(CHANGEMENT);
	while(1)
	{
	//	consigne1 = 3000;
	//	consigne2 = ASC_CONS_MAX;
// 		AX12GoTo (AX_ID2, 511);
// 		AX12GoTo (AX_ID1, 511);
// 		_delay_ms(CHANGEMENT);
	//	consigne1 = ASC_CONS_MAX;
	//	consigne2 = 3000;
		AX12GoTo (AX_ID2, 600);
		AX12GoTo (AX_ID1, 450);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 511);
		AX12GoTo (AX_ID1, 511);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 600);
		AX12GoTo (AX_ID1, 450);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 511);
		AX12GoTo (AX_ID1, 511);
		_delay_ms(CHANGEMENT);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT);
		AX12GoTo (AX_ID2, 600);
		AX12GoTo (AX_ID1, 450);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 511);
		AX12GoTo (AX_ID1, 511);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 600);
		AX12GoTo (AX_ID1, 450);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 511);
		AX12GoTo (AX_ID1, 511);
		_delay_ms(CHANGEMENT);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT);
		AX12GoTo (AX_ID2, 600);
		AX12GoTo (AX_ID1, 450);
		_delay_ms(PULSATION * 2);
		AX12GoTo (AX_ID2, 511);
		AX12GoTo (AX_ID1, 511);
		_delay_ms(CHANGEMENT);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT);
		AX12GoTo (AX_ID2, 600);
		AX12GoTo (AX_ID1, 450);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 511);
		AX12GoTo (AX_ID1, 511);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 600);
		AX12GoTo (AX_ID1, 450);
		_delay_ms(PULSATION);
		AX12GoTo (AX_ID2, 511);
		AX12GoTo (AX_ID1, 511);
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MAX;
		consigne2 = ASC_CONS_MED;
		_delay_ms(CHANGEMENT * 2);
		consigne1 = ASC_CONS_MED;
		consigne2 = ASC_CONS_MAX;
		_delay_ms(CHANGEMENT * 2);

		

	}

    return 0;
}

ISR( TIMER1_OVF_vect )
{
	asservissement();
}
