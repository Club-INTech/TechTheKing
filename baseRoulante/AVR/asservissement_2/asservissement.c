#include "asservissement.h"

void init (void)
{
    // Initialisation PWM pour les PH sur timer0
    // Initialisation pin 12
    DDRD |= ( 1 << PORTD6 );
    TCCR0A &= ~( 1 << COM0A0);
    TCCR0A |=  ( 1 << COM0A1 );
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
    
    // Initialisation ADC
    ADCSRA |= (1 << ADEN);

    // Interruptions
    sei();
    // Série
    uart_init();
    // I2C
    i2c_beginMaster();
    
}


/*
 * Contrôle des moteurs
 */


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


/*
 * Obtention des données des codeuses
 */


int32_t lireBuffer (uint8_t decalage)
{
    int32_t temp;
    int32_t temp2;
    
    temp = buffer[decalage];
    temp2 = buffer[decalage+1];
    temp += (temp2 << 8);
    temp2 = buffer[decalage+2];
    temp += (temp2 << 16);
    temp2 = buffer[decalage+3];
    temp += (temp2 << 24);

    return temp;
}

int32_t angle (void)
{
    i2c_requestFrom(2, 8);
    i = 0;

    while ( i2c_available() )
    {
        buffer[i] = i2c_receive();
        i++;
    }
    
    return lireBuffer(4);
}

int32_t distance (void)
{
    i2c_requestFrom(2, 8);
    i = 0;

    while ( i2c_available() )
    {
        buffer[i] = i2c_receive();
        i++;
    }

    return lireBuffer(0);
}