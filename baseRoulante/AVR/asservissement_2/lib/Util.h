#ifndef ACTIONNEURS_H
#define ACTIONNEURS_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"

/*
 *  Lecture analogique de la sortie sense des PHs
 *    int16_t entre 0 et 1024
 *    sert au contrôle du courant
 */
int16_t adc_sense1 (void); // PH 1
int16_t adc_sense2 (void); // PH 2

/*
 *  Fonctions pour récupérer les données de l'AVR compteur
 *  lireBuffer est appellée par les deux autres, dans l'idéal, ne pas l'utiliser
 */
int32_t lireBuffer (uint8_t decalage);
int32_t getAngle (void);
int32_t getDistance (void);

extern uint8_t buffer[];

#endif
