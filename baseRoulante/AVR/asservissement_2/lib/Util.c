#include "Util.h"

/*
 * Controle du courant des moteurs
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
