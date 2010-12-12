#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//set desired baud rate
#define BAUDRATE 2400
//calculate period rate
#define PERIOD_RATE 416 //1000000/(BAUDRATE*total/utile)
//define receive parameters
#define SYNC_BYTE 0XAA// synchro signal
#define RADDR_BYTE 0x44//Receiver address
#define SIG1_BYTE 0x11//signal 1
#define SIG2_BYTE 0x22//signal 2

#ifndef sbi
#define sbi(port,bit) (port) |= (1 << (bit))
#endif

#ifndef cbi
#define cbi(port,bit) (port) &= ~(1 << (bit))
#endif

#ifndef rbi
#define rbi(port,bit) ((port & (1 << bit)) >> bit)
#endif

