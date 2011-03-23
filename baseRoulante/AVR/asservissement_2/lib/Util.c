#include "Util.h"

uint8_t buffer[8] = {0};

void init (void)
{
    // Initialisation PWM pour le PH sur timer0
    // Initialisation pin 12
    DDRD |= ( 1 << PORTD6 );
    TCCR0A &= ~( 1 << COM0A0);
    TCCR0A |=  ( 1 << COM0A1 );
    // Fast PWM
    TCCR0A |= ( 1 << WGM00 );
    TCCR0A |= ( 1 << WGM01 );
    TCCR0B &= ~( 1 << WGM02 );
    // Prescaler /1024
    TCCR0B |= ( 1 << CS00 );
    TCCR0B &= ~( 1 << CS01 );
    TCCR0B |= ( 1 << CS02 );

    // Initialisation PWM pour le PH sur timer2
    // Initialisation pin 6
    DDRD |= ( 1 << PORTD3 );
    TCCR2A &= ~( 1 << COM2B0 );
    TCCR2A |= ( 1 << COM2B1 );
    // Fast PWM
    TCCR2A |= ( 1 << WGM20 );
    TCCR2A |= ( 1 << WGM21 );
    TCCR2B &= ~( 1 << WGM22 );
    // Prescaler /1024
    TCCR2B |= ( 1 << CS20 );
    TCCR2B |= ( 1 << CS21 );
    TCCR2B |= ( 1 << CS22 );

    // Pins de direction des PH
    DDRD |= ( 1 << PORTD4 );
    DDRB |= ( 1 << PORTB0 );

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


int16_t adc_sense1 (void)
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

int16_t adc_sense2 (void)
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

int32_t getAngle (void)
{
	uint8_t i;
    i2c_requestFrom(2, 8);
    i = 0;

    while ( i2c_available() )
    {
        buffer[i] = i2c_receive();
        i++;
    }
    
    return lireBuffer(4);
}

int32_t getDistance (void)
{
	uint8_t i;
    i2c_requestFrom(2, 8);
    i = 0;

    while ( i2c_available() )
    {
        buffer[i] = i2c_receive();
        i++;
    }

    return lireBuffer(0);
}
