#include "main.h"



ISR(INT0_vect) {
}

int main(void) {
	sbi(DDRB,PORTB5);
	//enable global interrupts
	sei();


	while(1) {
	}

	return 0;
}
