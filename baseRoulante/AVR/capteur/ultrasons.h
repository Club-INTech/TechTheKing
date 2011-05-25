#ifndef ULTRASONS_H
#define ULTRASONS_H

#include <avr/io.h>
#include <stdint.h>

/*
 *  Pins des ultrasons relies a la carte
 */
#define PIN_ULTRASON_1  (1 << PORTD2)
#define PIN_ULTRASON_2  (1 << PORTD3)
#define PIN_ULTRASON_3  (1 << PORTD6)

/*
 *  Temps maximal d'attente de la fin d'une impulsion
 *    S'exprimme en nombre de passage dans une boucle, donc depend de cette boucle
 */
#define TIMEOUT 1500

/*
 *  Fonction pour obtenir la distance a partir d'un capteur a ultrasons
 *    Prend la pin du capteur en argument
 */
uint16_t ping(uint8_t pin);

/*
 *  Valeurs enregistrees par les trois ultrasons
 */
extern volatile uint16_t ultra1;
extern volatile uint16_t ultra2;
extern volatile uint16_t ultra3;

#endif