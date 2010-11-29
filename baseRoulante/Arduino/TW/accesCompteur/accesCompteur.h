#ifndef AccesCompteur_h
#define AccesCompteur_h

#include <avr/interrupt.h>
#include <avr/io.h>
#include <Wire.h>

void compteurSetup();

int32_t distance();
int32_t angle();

#endif
