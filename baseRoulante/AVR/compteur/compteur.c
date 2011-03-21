#include "compteur.h"

void init (void)
{
    // Initialisation interruptions codeurs
    // Masques
    PCMSK2 |= (1 << PCINT18);
    PCMSK1 |= (1 << PCINT9);
    // Activer les interruptions
    PCICR |= (1 << PCIE2);
    PCICR |= (1 << PCIE1);

    // Interruptions
    sei();
    // I2C
    i2c_beginSlave(2);
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

void buffer_refresh (void)
{
    int32_t distance = roue1 + roue2;

    buffer[0] = (uint8_t) distance;
    buffer[1] = (uint8_t) (distance >> 8);
    buffer[2] = (uint8_t) (distance >> 16);
    buffer[3] = (uint8_t) (distance >> 24);

    int32_t angle = roue1 - roue2;

    buffer[4] = (uint8_t) angle;
    buffer[5] = (uint8_t) (angle >> 8);
    buffer[6] = (uint8_t) (angle >> 16);
    buffer[7] = (uint8_t) (angle >> 24);
}

void buffer_send()
{
    buffer_refresh();

    i2c_sendArray((unsigned char*) buffer, 8);
}