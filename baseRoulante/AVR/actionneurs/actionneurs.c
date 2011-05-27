#include <util/delay.h>

#include "actionneurs.h"
#include "ax12.h"

// Position des ascenseurs
volatile int16_t ascenseur1;
volatile int16_t ascenseur2;

// Consignes des ascenseurs
int16_t consigne1 = 0;
int16_t consigne2 = 0;
int16_t consigneb = 0;

// Mode d'asservissement ascenseurs
int8_t etat_asservissement = 0;

void init (void)
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
    
    // Initialisation ADC
    ADCSRA |= (1 << ADEN);
    
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
    
    // Ecrétage et application des PWM
    ASC_MOTEUR1 = (pwm1 <= ASC_PWM_MAX1)?pwm1:ASC_PWM_MAX1;
    ASC_MOTEUR2 = (pwm2 <= ASC_PWM_MAX2)?pwm2:ASC_PWM_MAX2;
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
	// Active l'asservissement du servo
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