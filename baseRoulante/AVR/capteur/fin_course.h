#ifndef FIN_COURSE_H
#define FIN_COURSE_H

#include <avr/io.h>
#include <stdint.h>

/*
 *  Pins des capteurs fin de course relies a la carte
 */
#define PIN_FINCOURSE_1  (1 << PORTB1)
#define PIN_FINCOURSE_2  (1 << PORTB2)

/*
 *  Fonction pour obtenir l'etat d'un capteur fin de course
 *    Prend la pin du capteur en argument
 */
uint8_t etat_bras(uint8_t pin);

#endif