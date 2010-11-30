#ifndef COMPTEUR_H
#define COMPTEUR_H

#include <avr/interrupt.h>
#include <avr/io.h> 

#define ENCGA (1 << PORTB2)
#define ENCGB (1 << PORTB4)
#define ENCDA (1 << PORTB3)
#define ENCDB (1 << PORTB5)

#define MASQUE B0111100

extern volatile int32_t encodeurG = 0;
extern volatile int32_t encodeurD = 0;
volatile char etatPins = 0;

void compteurSetup();
void compteurReset();

#endif 
