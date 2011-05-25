#include <avr/io.h>
#include <stdint.h>

#include "fin_course.h"

uint8_t etat_bras (uint8_t pin) {
    return (PINB & pin);
}