#ifndef ACTIONNEURS_H
#define ACTIONNEURS_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "i2c.h"
#include "serial.h"

/*
 *  Valeurs de comparaison pour le fast PWM
 *    entre 0 et 255
 */ 
#define MOTEUR2     OCR0A
#define MOTEUR1     OCR0B

/*
 *  Initialisations
 *    pins en sortie
 *    fast pwm
 *    timers
 *    i2c
 */
void init (void);

/*
 *  Lecture analogique de la sortie sense des PHs
 *    int entre 0 et 1024
 *    sert au contrôle du courant
 */
int adc_sense1 (void); // PH 1
int adc_sense2 (void); // PH 2

/*
 *  Variables et define pour l'i2c
 */
uint8_t buffer[4];

/*
 *  Variable de boucle
 */
uint8_t i;

/*
 *  Fonctions pour récupérer les données de l'AVR compteur
 *  lireBuffer est appellée par les deux autres, dans l'idéal, ne pas l'utiliser
 */
int32_t lireBuffer (uint8_t decalage);
int32_t angle (void);
int32_t distance (void);

/*
 * Coordonnees du robot
 */ 
#define CONVERSION_ANGLE 1000
#define CONVERSION_DISTANCE 1000

int32_t x,
int32_t y;

#endif
