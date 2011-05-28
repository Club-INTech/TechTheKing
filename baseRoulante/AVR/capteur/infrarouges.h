#ifndef INFRAROUGES_H
#define INFRAROUGES_H

#include <avr/io.h>
#include <stdint.h>

void infra_init( void );
uint16_t infra_distance1();
uint16_t infra_distance2();

#endif 
