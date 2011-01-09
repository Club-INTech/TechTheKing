#include "main.h"

volatile uint8_t transmetteur = 0b11111100;//bit 0 : info, bit 1 : flag, bits 2-7 : pointeur
Trame message = 0;

int main() {
	//On active la sortie sur le bon bit/port
	sbi(OUT_DDR,OUT_BIT);
	//On prepare le message a transmettre
	uint32_t contenu = (SYNC_BYTE << 23) + (COORD_X << 15) + COORD_Y;
	uint8_t cks = checksum(contenu);
	message = ( (contenu << 7) + cks );
	//On prépare le registre d'emission
	transmetteur = (message >> 39) & 1;//on veut que le premier bit.
	//On active le timer pour l'horloge d'emission sur overflow, taille réglable
	cbi(TCCR2A,WGM20);
	sbi(TCCR2A,WGM21);
	cbi(TCCR2B,WGM22);
	//Réglage du prescaler et de la valeur max du timer pour avoir 2400Hz
	cbi(TCCR2B,CS22);
	sbi(TCCR2B,CS21);
	sbi(TCCR2B,CS20);
	OCR2A=208;
	//Activation des interruptions sur overflow
	sbi(TIMSK2,TOIE2);//pour activer les interruptions sur overflow
	while(1) {
		if (transmetteur & (1 << 1)){
			if (transmetteur & 1) {
				sbi(OUT_PORT,OUT_BIT);
				cbi(transmetteur,1);
			}
			else {
				cbi(OUT_PORT,OUT_BIT);
				cbi(transmetteur,1);
			}
			_delay_us(PERIOD_RATE);
			if (transmetteur & 0) {
				sbi(OUT_PORT,OUT_BIT);
				cbi(transmetteur,1);
			}
			else {
				cbi(OUT_PORT,OUT_BIT);
				cbi(transmetteur,1);
			}
			_delay_us(PERIOD_RATE);
		}
	}
	return 0;
}

//Interruptions overflow timer2 -> transmission
ISR(TIMER2_OVF_vect) {
	uint8_t ptn = transmetteur >> 2;
	//on place le bon bit a emettre, on déplace le pointeur, et on active le flag pour emettre le fameux bit
	if (ptn > 0) {
		transmetteur = ((ptn-1) << 2) + ((message >> ptn) & 1 ) + (1 << 1);
	}
	else {
		transmetteur = (39 << 2) + ((message >> 39) & 1 ) + (1 << 1);
	}
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

