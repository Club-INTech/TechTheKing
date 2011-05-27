#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


void infra_init( void ) {
    // Initialisation ADC
    ADCSRA |= (1 << ADEN);
}


uint16_t infra_distance1() {
    // Sélectionne ADC1 pour la lecture
    ADMUX &= ~1;
    
    // Démarre la lecture
    ADCSRA |= (1 << ADSC);
    
    // On attend que ADSC passe à 0 (fin de la conversion)
    while (ADCSRA & (1 << ADSC));
    
    // On recompose le résultat et on le renvoie
    uint16_t low = ADCL;
    uint16_t high = ADCH;
    low += (high << 8);
    return low;
}


uint16_t infra_distance2() {
    // Sélectionne ADC1 pour la lecture
    ADMUX &= ~1;
    
    // Démarre la lecture
    ADCSRA |= (1 << ADSC);
    
    // On attend que ADSC passe à 0 (fin de la conversion)
    while (ADCSRA & (1 << ADSC));
    
    // On recompose le résultat et on le renvoie
    uint16_t low = ADCL;
    uint16_t high = ADCH;
    low += (high << 8);
    return low;
}
