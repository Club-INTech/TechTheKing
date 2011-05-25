#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
//#include <util/delay.h>
#include "../../common-AVR/usart.h"
#include "lib/temps.h"
#include "lib/lasers.h"


/**
 * d'abord quelques defines pour écrire et lire nos bits plus simplement.
 */
#ifndef sbi
#define sbi(port,bit) (port) |= (1 << (bit))
#endif

#ifndef cbi
#define cbi(port,bit) (port) &= ~(1 << (bit))
#endif

#ifndef rbi
#define rbi(port,bit) ((port & (1 << bit)) >> bit)
#endif

/**
 * ensuite les pins du moteur.
 */
#ifndef PINMOT
#define portTop PORTD
#define pinTop PORTD3
#define ddrMot1 DDRD 
#define portMot1 PORTD 
#define pinMot1 PORTD7
#define ddrMot2 DDRB 
#define portMot2 PORTB 
#define pinMot2 PORTB0 
#define ddrMot3 DDRB 
#define portMot3 PORTB 
#define pinMot3 PORTB1
#define ddrMot4 DDRB 
#define portMot4 PORTB 
#define pinMot4 PORTB2
#endif

/**
 * ensuite les pins pour le sélecteur de sens
 */
#ifndef PINSENS
#define portSens PORTC
#define pinSens PINC1 //pin 1 analog de l'arduino
#endif

void commuter(uint8_t pas_commuter[2]);
void pwm_init();

#endif
