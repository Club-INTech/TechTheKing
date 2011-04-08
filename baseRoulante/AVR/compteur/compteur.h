#ifndef COMPTEUR_H
#define COMPTEUR_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "twi_slave.h"

/*
 *  Pins des codeurs
 */
#define CODEUR11    (1 << PORTD2)
#define CODEUR12    (1 << PORTD3)
#define CODEUR21    (1 << PORTC1)
#define CODEUR22    (1 << PORTC0)

/*
 *  Position roues
 */
volatile int32_t roue1;
volatile int32_t roue2;

/*
 *  Initialisations
 *    interruptions codeurs
 */
void compteur_init (void);

/*
 *  Fonctions pour mettre l'angle ou la distance dans le buffer
 *    le bit de poids faible est en début de tableau (envoyé en premier sur le bus i2c)
 */
void charger_distance (void);
void charger_angle (void);

#endif
