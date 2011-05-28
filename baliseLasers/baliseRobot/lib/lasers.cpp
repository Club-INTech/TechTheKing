#include "lasers.h"

void lasers_init() {
	sbi(DDRB,PORTB3);	//définie la sortie A du timer2
	OCR2A=255;
	/* positionner le mode pwm */
	sbi(TCCR2A,WGM20);	//
	sbi(TCCR2A,WGM21);	//fast-pwm, TOP=OCR2A
	cbi(TCCR2B,WGM22);	//
	/* paramétrer le comportement sur comparaisons */
	sbi(TCCR2A,COM2A1);	//fast-pwm -> OC2A mis à 0 lors d'une comparaison
	cbi(TCCR2A,COM2A0);	//réussie et mis à 1 à passage par BOTTOM
	/* sélection de la source du timer */
	cbi(TCCR2B,CS22);	//
	sbi(TCCR2B,CS21);	//prescaler à 8
	cbi(TCCR2B,CS20);	//
	/**
	cbi(TCCR2B,CS22);	//
	cbi(TCCR2B,CS21);	//pas de prescaler
	sbi(TCCR2B,CS20);	//
	*/
	return ;
}

void lasers_start() {
	OCR2A=128;
}

void lasers_stop() {
	OCR2A=255;
}
