#include <avr/io.h>
#include <avr/interrupt.h>
#include "actionneurs.h"
#include "ax12.h"

void init (void)
{
    // Initialisation PWM pour les servos sur timer0
    // Initialisation pin 12
    DDRD |= ( 1 << PORTD6 );
    TCCR0A &= ~( 1 << COM0A0);
    TCCR0A |= ( 1 << COM0A1 );
    // Initialisation pin 11
    DDRD |= ( 1 << PORTD5 );
    TCCR0A &= ~( 1 << COM0B0 );
    TCCR0A |= ( 1 << COM0B1 );
    // Fast PWM
    TCCR0A |= ( 1 << WGM00 );
    TCCR0A |= ( 1 << WGM01 );
    TCCR0B &= ~( 1 << WGM02 );
    // Prescaler /1024
    TCCR0B |= ( 1 << CS00 );
    TCCR0B &= ~( 1 << CS01 );
    TCCR0B |= ( 1 << CS02 );
    
    // Initialisation PWM pour les PH sur timer2
    // Initialisation pin 17
    DDRB |= ( 1 << PORTB3 );
    TCCR2A &= ~( 1 << COM2A0);
    TCCR2A |= ( 1 << COM2A1 );
    // Initialisation pin 5
    DDRD |= ( 1 << PORTD3 );
    TCCR2A &= ~( 1 << COM2B0 );
    TCCR2A |= ( 1 << COM2B1 );
    // Fast PWM
    TCCR2A |= ( 1 << WGM20 );
    TCCR2A |= ( 1 << WGM21 );
    TCCR2B &= ~( 1 << WGM22 );
    // Prescaler /1024
    TCCR2B |= ( 1 << CS20 );
    TCCR2B &= ~( 1 << CS21 );
    TCCR2B |= ( 1 << CS22 );
    
    // Initialisation ADC
    ADCSRA |= (1 << ADEN);
    
    // Initialisation interruptions codeurs
    // Masques
    PCMSK1 |= (1 << PCINT10);
    PCMSK0 |= (1 << PCINT0);
    // Activer les interruptions sur les ports C et B
    PCICR |= (1 << PCIE2);
    PCICR |= (1 << PCIE1);
    sei();
}

int adc_sense1 (void)
{
    // Sélectionne ADC1 pour la lecture
    ADMUX |= 1;
    
    // Démarre la lecture
    ADCSRA |= (1 << ADSC);
    
    // On attend que ADSC passe à 0 (fin de la conversion)
    while (ADCSRA & (1 << ADSC));
    
    // On recompose le résultat et on le renvoie
    return (ADCH | ADCL);
}

int adc_sense2 (void)
{
    // Sélectionne ADC0 pour la lecture
    ADMUX &= ~1;
    
    // Démarre la lecture
    ADCSRA |= (1 << ADSC);
    
    // On attend que ADSC passe à 0 (fin de la conversion)
    PCMSK1 |= (1 << PCINT11);
    while (ADCSRA & (1 << ADSC));
    
    // On recompose le résultat et on le renvoie
    return (ADCH | ADCL);
}

void AX12Init (uint8_t ID, uint16_t angleCW, uint16_t angleCCW, uint16_t vitesse)
{
	// Active l'asservissement
	writeData (ID, AX_TORQUE_ENABLE, 1, 1);
	// Définit les angles mini et maxi
	writeData (ID, AX_CW_ANGLE_LIMIT_L, 2, angleCW);
	writeData (ID, AX_CCW_ANGLE_LIMIT_L, 2, angleCCW);
	// Définit la vitesse de rotation
	writeData (ID, AX_GOAL_SPEED_L, 2, vitesse);
	// Fonction bas niveau pour la transmission série
	ax12Init (1000000);
}

void AX12GoTo (uint8_t ID, uint16_t angle)
{
	writeData (ID, AX_GOAL_POSITION_L, 2, angle);
}

// Interruption codeur 1
ISR (PCINT0_vect)
{
    // Front montant codeur 1
    if ( PINB & CODEUR11 )
    {
        if ( PINB & CODEUR12 )
            ascenseur1++;
    }
    // Front descendant codeur 1
    else
    {
        if ( PINB & CODEUR12 )
            ascenseur1--;
    }
}

// Interruption codeur 2
ISR (PCINT1_vect)
{
    // Front montant codeur 2
    if ( PIND & CODEUR21 )
    {
        if ( PIND & CODEUR22 )
            ascenseur2++;
    }
    // Front descendant codeur 2
    else
    {
        if ( PIND & CODEUR22 )
            ascenseur2--;
    }
}