#ifndef COMI2C_H
#define COMI2C_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include "Wire.h"

typedef enum {DISTANCE, ANGLE} TypeReponse;

void comTWSetup();

unsigned char typeReponse = 0;

#endif 
