#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "capteurs.h"

uint32_t ultrason;

uint32_t ping(uint8_t pin)
{
    // Envoi d'une impulsion dans le capteur
    DDRD |= pin;
    PORTD &= ~pin;
    _delay_us(2);
    PORTD |= pin;
    _delay_us(5);
    PORTD &= ~pin;

    // Reception de la duree
    DDRD &= ~pin;

    uint8_t masque = pin;

    uint32_t duree = 0;
    uint32_t duree_max = TIMEOUT;
    
    // Attente de la fin de l'impulsion precedente
    while ((PIND & pin) == masque)
        if (duree++ == duree_max)
            return 0;
        
    // Attente du demarrage de l'impulsion
    while ((PIND & pin) != masque)
        if (duree++ == duree_max)
            return 0; 
        
    // Attente de la fin de l'impulsion
    while ((PIND & pin) == masque)
            duree++;
    
    return duree;
}
