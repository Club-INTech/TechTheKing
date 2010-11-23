#ifndef LASERS_H
#define LASERS_H

#include <avr/io.h>

/**
 * tout d'abord quelques defines pour écrire et lire nos bits plus simplement.
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

#endif

/**
 * définition des fonctions qu'on va utiliser
 */
void lasers_init();
