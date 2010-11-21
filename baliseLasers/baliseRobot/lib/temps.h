#ifndef TEMPS_H
#define TEMPS_H

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef sbi
#define sbi(port,bit) (port) |= (1 << (bit))
#endif

#ifndef cbi
#define cbi(port,bit) (port) &= ~(1 << (bit))
#endif


void temps_init();
uint32_t micros();

#endif
