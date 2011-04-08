#include "compteur.h"
#include "twi_slave.h"

volatile int32_t roue1;
volatile int32_t roue2;

void compteur_init (void)
{
    // Initialisation interruptions codeurs
    // Masques
    PCMSK2 |= (1 << PCINT18);
    PCMSK1 |= (1 << PCINT9);
    // Activer les interruptions
    PCICR |= (1 << PCIE2);
    PCICR |= (1 << PCIE1);
}

// Interruption codeur 1
ISR (PCINT2_vect)
{
    // Front montant codeur 1
    if ( PIND & CODEUR11 )
    {
        if ( PIND & CODEUR12 )
            roue1--;
    }
    // Front descendant codeur 1
    else
    {
        if ( PIND & CODEUR12 )
            roue1++;
    }
}

// Interruption codeur 2
ISR (PCINT1_vect)
{
    // Front montant codeur 2
    if ( PINC & CODEUR21 )
    {
        if ( PINC & CODEUR22 )
            roue2--;
    }
    // Front descendant codeur 2
    else
    {
        if ( PINC & CODEUR22 )
            roue2++;
    }
}

void charger_distance (void)
{
    int32_t distance = roue1 + roue2;

    messageBuf[0] = (uint8_t) distance;
    messageBuf[1] = (uint8_t) (distance >> 8);
    messageBuf[2] = (uint8_t) (distance >> 16);
    messageBuf[3] = (uint8_t) (distance >> 24);
}

void charger_angle (void)
{
    int32_t angle = roue1 - roue2;

    messageBuf[0] = (uint8_t) angle;
    messageBuf[1] = (uint8_t) (angle >> 8);
    messageBuf[2] = (uint8_t) (angle >> 16);
    messageBuf[3] = (uint8_t) (angle >> 24);
}
