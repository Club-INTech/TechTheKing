#ifndef CAPTEURS_H
#define CAPTEURS_H

#include <avr/io.h>
#include <stdint.h>

/*
 *  Pins des ultrasons relies a la carte
 */
#define PIN_ULTRASON  (1 << PORTD6)

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
extern uint16_t ultrason;

/*
 *  Pins des capteurs tout ou rien
 */
#define PIN_BRAS1  (1 << PORTC1)
#define PIN_BRAS2  (1 << PORTC2)
#define PIN_JUMPER  (1 << PORTD5)

/*
 *  Fonction pour obtenir l'etat d'un capteur tout ou rien
 *    Prend la pin du capteur en argument
 */
uint8_t etat_capteur(uint8_t pin);

#endif