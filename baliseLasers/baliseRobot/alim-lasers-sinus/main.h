#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include "../../../common-AVR/usart.h"
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

#endif
