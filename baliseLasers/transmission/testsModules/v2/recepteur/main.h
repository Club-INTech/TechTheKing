#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../../../../../common-AVR/usart.h"

//set output pin : 
#define OUT_DDR DDRD
#define OUT_PIN PIND
#define OUT_PORT PORTD
#define OUT_BIT PORTD2

//set desired baud rate
#define BAUDRATE 2400
//calculate period rate
#define PERIOD_RATE 416 //1000000/(BAUDRATE*total/utile)
//define receive parameters
#define BASE_BYTE 0xAA//Byte de base
#define SYNC_BYTE 0xD7//synchro signal//11010111
#define COORD_X 0x149//signal 1
#define COORD_Y 0x37B//signal 2

typedef uint64_t Trame;

#ifndef sbi
#define sbi(port,bit) (port) |= (1 << (bit))
#endif

#ifndef cbi
#define cbi(port,bit) (port) &= ~(1 << (bit))
#endif


// Declaration des fonctions
uint8_t checksum(uint32_t data);
