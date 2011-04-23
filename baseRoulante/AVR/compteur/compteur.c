#include "compteur.h"
#include "twi_slave.h"

volatile int32_t roue1;
volatile int32_t roue2;
volatile uint8_t pins_codeuses;

void compteur_init (void)
{
    // Initialisation interruptions codeurs roues
    // Masques
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT8);
    // Activer les interruptions roues
    PCICR |= (1 << PCIE2);
    PCICR |= (1 << PCIE1);
    // Initialisation de l'état des pins des codeuses
    pins_codeuses = (PORTC & (CODEUR21 | CODEUR22)) | (PORTD & (CODEUR11 | CODEUR12));
    
}

// Interruption codeur 1
ISR (PCINT2_vect)
{
    // Etat des pins à l'interruption précédente
    uint8_t pin1 = pins_codeuses & CODEUR11;
    uint8_t pin2 = pins_codeuses & CODEUR12;

    pins_codeuses = (PORTC & (CODEUR21 | CODEUR22)) | (PORTD & (CODEUR11 | CODEUR12));

    // Enumération des différents cas possibles lors d'une interruption
    if ( !pin1 & !pin2 ) {
        if ( PIND & (!CODEUR11 & CODEUR12) )
            roue1++;
        else
            roue1--;
    }
    else if ( pin1 & !pin2 ) {
        if ( PIND & (!CODEUR11 & !CODEUR12) )
            roue1++;
        else
            roue1--;
    }
    else if ( !pin1 & pin2 ) {
        if ( PIND & (CODEUR11 & CODEUR12) )
            roue1++;
        else
            roue1--;
    }
    else {
        if ( PIND & (CODEUR11 & !CODEUR12) )
            roue1++;
        else
            roue1--;
    }
}

// Interruption codeur 2
ISR (PCINT1_vect)
{
    // Etat des pins à l'interruption précédente
    uint8_t pin1 = pins_codeuses & CODEUR21;
    uint8_t pin2 = pins_codeuses & CODEUR22;

    pins_codeuses = (PINC & (CODEUR21 | CODEUR22)) | (PORTD & (CODEUR11 | CODEUR12));

    // Enumération des différents cas possibles lors d'une interruption
    if ( !pin1 & !pin2 ) {
        if ( PINC & (!CODEUR21 & CODEUR22) )
            roue2++;
        else
            roue2--;
    }
    else if ( pin1 & !pin2 ) {
        if ( PINC & (!CODEUR21 & !CODEUR22) )
            roue2++;
        else
            roue2--;
    }
    else if ( !pin1 & pin2 ) {
        if ( PINC & (CODEUR21 & CODEUR22) )
            roue2++;
        else
            roue2--;
    }
    else {
        if ( PINC & (CODEUR21 & !CODEUR22) )
            roue2++;
        else
            roue2--;
    }
}
