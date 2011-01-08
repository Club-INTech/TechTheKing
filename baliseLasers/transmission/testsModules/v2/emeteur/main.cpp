#include "main.h"
uint8_t bit = 0;//bit 0 : info, bit 1 : flag
int main() {
	sbi(OUT_DDR,OUT_BIT);
	while(1) {
		if (bit & (1 << 1)){
			if (bit & 1) {
				sbi(OUT_PORT,OUT_BIT);
			}
			else {
				cbi(OUT_PORT,OUT_BIT);
			}
			_delay_us(PERIOD_RATE);
			if (bit & 0) {
				sbi(OUT_PORT,OUT_BIT);
			}
			else {
				cbi(OUT_PORT,OUT_BIT);
			}
			_delay_us(PERIOD_RATE);
		}
	}
	return 0;
}

uint8_t checksum(uint32_t data) {
	return ((uint8_t)data + (uint8_t)(data << 8) + (uint8_t)(data << 16) + (uint8_t)(data << 24));
}

/* utilite faible, calcul lourd?
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

