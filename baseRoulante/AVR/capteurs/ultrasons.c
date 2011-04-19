#include "ultrasons.h"

uint16_t ping(uint8_t pin)
{
    // Envoi d'une impulsion dans le capteur
    DDRB |= pin;
    PORTB &= ~pin;
    _delay_us(2);
    PORTB |= pin;
    _delay_us(5);
    PORTB &= ~pin;

    // Reception de la duree
    DDRB &= ~pin;

    uint8_t masque = pin;

    uint16_t duree = 0;
    uint16_t duree_max = TIMEOUT;

    // Attente de la fin de l'impulsion precedente
    while ((PINB & pin) == masque)
        if (duree++ == duree_max)
            return 0;

    // Attente du demarrage de l'impulsion
    while ((PINB & pin) != masque)
        if (duree++ == duree_max)
            return 0;

    // Attente de la fin de l'impulsion
    while ((PINB & pin) == masque)
            duree++;

    return duree;
} 
