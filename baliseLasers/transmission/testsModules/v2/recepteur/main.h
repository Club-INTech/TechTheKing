#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lib/temps.h"
#include "../../../../../common-AVR/usart.h"

//set output pin : 
#define OUT_DDR DDRD
#define OUT_PIN PIND
#define OUT_PORT PORTD
#define OUT_BIT PORTD2

//set desired baud rate
#define BAUDRATE 2400
//calculate period rate
#define PERIOD_RATE 417 //1000000/(BAUDRATE*total/utile)
#define PERIOD_RATE_FAST 139
//define receive parameters
#define BASE_BYTE 0xAA//Byte de base
#define SYNC_BYTE 0xD7//synchro signal//11010111//etudier des bytes de synchro adaptes a la methode de detection (juste apres un CHANGE)
#define COORD_X 0x149//signal 1
#define COORD_Y 0x37B//signal 2

//flags pour l'octet transmetteur : 
#define FLAG_INIT_BIT 0
#define FLAG_INIT_TRAME 1
#define FLAG_FIN_TRAME 2
#define FLAG_POINTEUR_MESSAGE 3
#define FLAG_MASQUE 0b111

//le type Trame
typedef uint64_t Trame;

//les sbi et cbi, hyper-pratiques, améliorent la lisibilité.
#ifndef sbi
#define sbi(port,bit) (port) |= (1 << (bit))
#endif

#ifndef cbi
#define cbi(port,bit) (port) &= ~(1 << (bit))
#endif


// Declaration des fonctions
//uint8_t checksum(uint32_t data);
