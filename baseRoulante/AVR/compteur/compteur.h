#ifndef COMPTEUR_H
#define COMPTEUR_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c.h"

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
 *  Buffer l'i2c
 */
uint8_t buffer[8];

/*
 *  Initialisations
 *    interruptions codeurs
 */
void init (void);

/*
 *  Fonctions pour mettre l'angle puis la distance dans le buffer
 *    le bit de poids faible est en début de tableau (envoyé en premier sur le bus i2c)
 */
void buffer_refresh(void);

/*
 * Fonction de réponse aux requêtes du master
 */
void buffer_send();

#endif
