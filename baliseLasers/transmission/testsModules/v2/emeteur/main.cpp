/*
 * emetteur.cpp
 */

#include "main.h"

volatile uint8_t transmetteur = 0b11111100;//bit 0 : info, bit 1 : flag, bits 2-7 : pointeur
Trame message = 0;

volatile uint8_t bit = 0;
/*
   uint8_t checksum(uint32_t data) {
   return ((uint8_t)data + (uint8_t)(data << 8) + (uint8_t)(data << 16) + (uint8_t)(data << 24));
   }
   */
//Et une macro pour la route
#define checksum(data) ((uint8_t)data + (uint8_t)(data >> 8) + (uint8_t)(data >> 16) + (uint8_t)(data >> 24))

int main() {
	//on initialise le debug serial
	uart_init();
	printlnString("Start");

	//On active la sortie sur le bon bit/port
	sbi(OUT_DDR,OUT_BIT);
	sbi(DDRB,PORTB3);

	//On prepare le message a transmettre
	uint32_t contenu = (((uint32_t)SYNC_BYTE << 24) + ((uint32_t)COORD_X << 16) + (uint32_t)COORD_Y);
	uint8_t cks = checksum(contenu);
	message = ( (contenu << 8) + cks );

	//On prépare le registre d'emission
	transmetteur = (message >> 40) & 1;//on veut que le premier bit.

	/*
	 * 20MHz
	//On active le timer pour l'horloge d'emission sur overflow, taille réglable
	cbi(TCCR0A,WGM00);
	sbi(TCCR0A,WGM01);
	cbi(TCCR0B,WGM02);
	//Réglage du prescaler et de la valeur max du timer pour avoir 2400Hz
	cbi(TCCR0B,CS02);
	sbi(TCCR0B,CS01);
	sbi(TCCR0B,CS00);
	OCR0A=130;
	*/
	/*
	 * 16MHz
	 */
	//On active le timer pour l'horloge d'emission sur overflow, taille réglable
	cbi(TCCR0A,WGM00);
	sbi(TCCR0A,WGM01);
	cbi(TCCR0B,WGM02);
	//Réglage du prescaler et de la valeur max du timer pour avoir 2400Hz
	/*cbi(TCCR0B,CS02);
	sbi(TCCR0B,CS01);
	sbi(TCCR0B,CS00);*/
	cbi(TCCR0B,CS02);
	sbi(TCCR0B,CS01);
	cbi(TCCR0B,CS00);
	OCR0A=104;

	//Activation des interruptions sur overflow
	sbi(TIMSK0,OCIE0A);//pour activer les interruptions sur overflow

	//et on y va 
	//printlnLong(contenu);
	printlnString("Go");

	while(1) {
		//controle du flag d'emission
		if (transmetteur & (1 << 1)){

			//on emet le bit et on retire le flag d'emission
			if (transmetteur & 1) {
				sbi(OUT_PORT,OUT_BIT);
				sbi(PORTB,PORTB3);
				printString("1");
			}
			else {
				cbi(OUT_PORT,OUT_BIT);
				cbi(PORTB,PORTB3);
				printString("0");
			}
			cbi(transmetteur,1);
			//on attend notre prériode (utilité?test?bloquant!)
			/*_delay_us(PERIOD_RATE);
			  if (transmetteur & 0) {//GNÉ????? grosse fumette!!!
			  sbi(OUT_PORT,OUT_BIT);
			  sbi(PORTB,PORTB3);
			  }
			  else {
			  cbi(OUT_PORT,OUT_BIT);
			  cbi(PORTB,PORTB3);
			  }
			//on attend notre prériode (utilité?test?bloquant!)
			_delay_us(PERIOD_RATE);*/
		}
	}
	return 0;
}

//Interruptions overflow timer2 -> transmission
ISR(TIMER0_COMPA_vect) {
	if (bit == 0){
		bit = 1;
		sbi(PORTB,PORTB3);
	}
	else{
		cbi(PORTB,PORTB3);
		bit=0;
	}
	//printlnString(".");/*
	uint8_t ptn = transmetteur >> 2;
	//on place le bon bit a emettre, on déplace le pointeur, et on active le flag pour emettre le fameux bit
	if (ptn > 0) {
		transmetteur = ((ptn-1) << 2) + ((message >> ptn) & 1 ) + (1 << 1);
	}
	else {
		//transmetteur = (40 << 2) + ((message >> 40) & 1 ) + (1 << 1);
		transmetteur = (48 << 2) + ((message >> 48) & 1 ) + (1 << 1);
	}
}
