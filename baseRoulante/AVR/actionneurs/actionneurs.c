#include <util/delay.h>

#include "actionneurs.h"
#include "ax12.h"

// Position des ascenseurs
volatile int16_t ascenseur1;
volatile int16_t ascenseur2;

// Consignes des ascenseurs
volatile int16_t consigne1 = 0;
volatile int16_t consigne2 = 0;
volatile int16_t consigneb = 0;

uint16_t tempo1=0,tempo2=0;

// Mode d'asservissement ascenseurs
volatile int8_t etat_asservissement = 0;

void actio_init (void)
{
	// Initialisation PWM pour les servos sur timer0
	// Initialisation pin 12
	DDRD |= (1 << PORTD6);
	TCCR0A &= ~(1 << COM0A0);
	TCCR0A |= (1 << COM0A1);
	// Initialisation pin 11
	DDRD |= (1 << PORTD5);
	TCCR0A &= ~(1 << COM0B0);
	TCCR0A |= (1 << COM0B1);
	// Fast PWM
	TCCR0A |= (1 << WGM00);
	TCCR0A |= (1 << WGM01);
	TCCR0B &= ~(1 << WGM02);
	// Prescaler /1024
	TCCR0B |= (1 << CS00) | (1 << CS02);

	// Initialisation PWM pour les PH sur timer2
	// Initialisation pin 17
	DDRB |= (1 << PORTB3);
	TCCR2A &= ~(1 << COM2A0);
	TCCR2A |= (1 << COM2A1);
	// Initialisation pin 5
	DDRD |= (1 << PORTD3);
	TCCR2A &= ~(1 << COM2B0);
	TCCR2A |= (1 << COM2B1);
	// Fast PWM
	TCCR2A |= (1 << WGM20);
	TCCR2A |= (1 << WGM21);
	TCCR2B &= ~(1 << WGM22);
	// Prescaler /1
	TCCR2B |= (1 << CS20);

	// Pins de direction moteurs en sortie
	DDRD |= DIR1;
	DDRD |= DIR2;

	// Initialisation interruptions codeurs
	// Masques
	PCMSK1 |= (1 << PCINT10);
	PCMSK0 |= (1 << PCINT1);
	// Activer les interruptions sur les ports C et B
	PCICR |= (1 << PCIE1);
	PCICR |= (1 << PCIE0);
	// Pull-ups
	PORTB |= CODEUR22 | CODEUR21;
	PORTC |= CODEUR12 | CODEUR11;

	// Activation globale des interruptions
	sei();
}

void asservissement (void)
{
	// Calcul des PWM
	int32_t pwm1 = (consigne1 - ascenseur1)*ASC_KP1;
	int32_t pwm2 = (consigne2 - ascenseur2)*ASC_KP2;

	// Gestion du signe
	if (pwm1 < 0 ) {
		pwm1 = -pwm1;
		PORTD |= DIR1;
	}
	else
		PORTD &= ~DIR1;

	if (pwm2 < 0 ) {
		pwm2 = -pwm2;
		PORTD &= ~DIR2;
	}
	else
		PORTD |= DIR2;


	if( (unsigned long)adcRead(ADC0) >= CUR_LIM )	//Si l'on depasse le courant limite
	{
		if( tempo1 >= TEMPS_MAX )	//Plus d'un certains temps
			pwm2--;			//On décrémente le pwm
		else				//Par contre, si le temps max n'est pas atteint
			tempo1++;		//On incrémente le temps
	}
	else					//Mais si le courant limite n'est pas (ou plus) dépassé
	{
		if( tempo1 >= TEMPS_MAX )	//et que le temps max a était atteint
			tempo1 = 0;		//On le reset
	}
	
	if( (unsigned long)adcRead(ADC1) >= CUR_LIM )
	{
		if( tempo2 >= TEMPS_MAX )
			pwm1--;
		else
			tempo2++;
	}
	else
	{
		if( tempo2 >= TEMPS_MAX )
			tempo2 = 0;
	}
	
	if( (unsigned long)adcRead(ADC0) >= CUR_MAX )	//Si l'on dépasse le courant maximum
		pwm2=0;					//Alors on coupe carrement le pwm
	if( (unsigned long)adcRead(ADC1) >= CUR_MAX )
		pwm1=0;

	// Ecrétage et application des PWM
	ASC_MOTEUR1 = (pwm1 <= ASC_PWM_MAX1)?pwm1:ASC_PWM_MAX1;
	ASC_MOTEUR2 = (pwm2 <= ASC_PWM_MAX2)?pwm2:ASC_PWM_MAX2;
}

void asservissement_synchro (void)
{
	// L'un des moteurs est asservi sur la position de l'autre

	// Calcul des PWM
	int32_t pwm1 = (consigneb - ascenseur1)*ASC_KP1;
	int32_t pwm2 = (ascenseur1 - ascenseur2)*ASC_KP2;

	// Gestion du signe
	if (pwm1 < 0 ) {
		pwm1 = -pwm1;
		PORTD |= DIR1;
	}
	else
		PORTD &= ~DIR1;

	if (pwm2 < 0 ) {
		pwm2 = -pwm2;
		PORTD &= ~DIR2;
	}
	else
		PORTD |= DIR2;

	if( (unsigned long)adcRead(ADC0) >= CUR_LIM )	//Si l'on depasse le courant limite
	{
		if( tempo1 >= TEMPS_MAX )	//Plus d'un certains temps
			pwm2--;			//On décrémente le pwm
		else				//Par contre, si le temps max n'est pas atteint
			tempo1++;		//On incrémente le temps
	}
	else					//Mais si le courant limite n'est pas (ou plus) dépassé
	{
		if( tempo1 >= TEMPS_MAX )	//et que le temps max a était atteint
			tempo1 = 0;		//On le reset
	}

	if( (unsigned long)adcRead(ADC1) >= CUR_LIM )
	{
		if( tempo2 >= TEMPS_MAX )
			pwm1--;
		else
			tempo2++;
	}
	else
	{
		if( tempo2 >= TEMPS_MAX )
			tempo2 = 0;
	}

	if( (unsigned long)adcRead(ADC0) >= CUR_MAX )	//Si l'on dépasse le courant maximum
		pwm2=0;					//Alors on coupe carrement le pwm
	if( (unsigned long)adcRead(ADC1) >= CUR_MAX )
		pwm1=0;
	
	// Ecrétage et application des PWM
	ASC_MOTEUR1 = (pwm1 <= ASC_PWM_MAX1)?pwm1:ASC_PWM_MAX1;
	ASC_MOTEUR2 = (pwm2 <= ASC_PWM_MAX2)?pwm2:ASC_PWM_MAX2;
}

// Interruption codeur 2
ISR (PCINT0_vect)
{
	if (PINB & CODEUR21){
		if (PINB & CODEUR22)
			ascenseur2++;
		else
			ascenseur2--;
	}
}

// Interruption codeur 1
ISR (PCINT1_vect)
{
	if (PINC & CODEUR11){
		if (PINC & CODEUR12)
			ascenseur1--;
		else
			ascenseur1++;
	}
}

void recalage (void)
{
	// Tres sale, a ameliorer
	etat_asservissement = ASC_ASSERV_INDEP;
	PORTD &= ~DIR2;
	PORTD |= DIR1;
	ASC_MOTEUR1 = ASC_PWM_MAX1-50;
	ASC_MOTEUR2 = ASC_PWM_MAX2-50;
	_delay_ms(2500);
	ascenseur1 = 0;
	ascenseur2 = 0;
	consigne1 = 0;
	consigne2 = 0;
}
