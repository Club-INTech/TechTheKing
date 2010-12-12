#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


//set desired baud rate
#define BAUDRATE 2400
//calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)
//define receive parameters
#define SYNC_BYTE 0XAA// synchro signal
#define RADDR_BYTE 0x44
#define LEDON_BYTE 0x11//LED on command
#define LEDOFF_BYTE 0x22//LED off command

#ifndef sbi
#define sbi(port,bit) (port) |= (1 << (bit))
#endif

#ifndef cbi
#define cbi(port,bit) (port) &= ~(1 << (bit))
#endif

