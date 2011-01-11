#include "main.h"

volatile uint8_t transmetteur = 0b11111000;//bit 0 : flag bit reçu, bit 1 : flag init trame, bit 2 : flag fin trame, bits 3-7 : pointeur
//Trame message = 0;
uint32_t message = 0;
uint8_t init = 0;
volatile uint32_t temps[2] = {0,0};
uint32_t mesures=0;
uint32_t nb_bit=0;
uint8_t ind_mesures=0;

int main() {
	//on initialise la transmission série (pour le debug)
	uart_init();

	//On active la sortie sur le bon bit/port
	sbi(OUT_DDR,OUT_BIT);
	//Activation des interruptions d'entree on CHANGE
	cbi(EICRA,ISC01);
	sbi(EICRA,ISC00);
	sbi(EIMSK,INT0);
	temps[0] = micros();
	while(1) {
		// On travaille en échantillonage
		//si changement de bit 
		if (transmetteur & 1){
			//on prend la main
			cbi(transmetteur,0);
			//on stocke le nombre de bits lues
			nb_bit = (temps[0]-temps[1])/PERIOD_RATE;
			//on enregistre le nombre de bit correspondant
			uint8_t ptn = transmetteur >> 3;
			if ((OUT_PIN >> OUT_BIT) & 1) {
				while (nb_bit) {
					cbi(message,ptn-nb_bit);
					nb_bit--;
				}
			}
			else {
				while (nb_bit) {
					sbi(message,ptn-nb_bit);
					nb_bit--;
				}
			}
			//On décale le pointeur de message, et si l'en-tête est bonne, on place le flag pour demarrer l'analyse de la trame.
			if ((ptn-nb_bit <= 24) && ((transmetteur >> 1) & 1)) {
				if (((message >> 24) & 0xFF) == SYNC_BYTE) {
					sbi(transmetteur,1);
				}
				transmetteur = (transmetteur & 0b111) + 0xF8;
			}
			else if (ptn <= nb_bit) {//!!!risque d'écraser le début de trame
				sbi(transmetteur,2);
				transmetteur = (transmetteur & 0b111) + 0xF8;
			}
			else {

				transmetteur = (transmetteur & 0b111) + ((ptn-nb_bit)<<3);
			}
			//
			//
			//
		}
		//fin de trame?
		if ((transmetteur >> 2) & 1){
			uint8_t ptn = transmetteur >> 3;
			if (ptn>0) {
				transmetteur = ((ptn-1) << 3);
			}
			else {
				transmetteur = (32 << 2);
				printUInt((message >> 19) & 0xFFF);
				printString(" ");
				printUInt((message >> 7) & 0xFFF);
				printString(" ");
				printUShort(message & 0xFF);
				printString(" ");
				uint8_t tmp = checksum((message >> 8) & 0xFFFFFFFF);
				printUShort(tmp);
				printString(" ");
				printlnUShort(tmp == (message & 0xFF));
				message = 0;
			}
		}
	}
	return 0;
}

//Interruptions overflow timer2 -> transmission
ISR(INT0_vect) {
	// On informe du changement
	sbi(transmetteur,0);
	// On stoque la durée depuis le dernier bit
	temps[1] = temps[0];
	temps[0] = micros();
}

uint8_t checksum(uint32_t data) {
	return ((uint8_t)data + (uint8_t)(data << 7) + (uint8_t)(data << 15) + (uint8_t)(data << 23));
}

/* utilite faible, calcul lourd?, le checksum a été préféré
   uint8_t crc(uint32_t message) {

#define POLYNOMIAL 0xD8000000  // 11011 followed by 0's 
uint32_t  remainder;	
// Initially, the dividend is the remainder.
remainder = message;
// For each bit position in the message....
for (uint8_t bit = 32; bit > 0; --bit)
{
// If the uppermost bit is a 1...
if (remainder & 0x80)
{
// XOR the previous remainder with the divisor.
remainder ^= POLYNOMIAL;
}
// Shift the next bit of the message into the remainder.
remainder = (remainder << 1);
}
// Return only the relevant bits of the remainder as CRC.
return (remainder >> 24);

}*/

