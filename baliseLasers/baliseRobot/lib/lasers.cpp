#include "lasers.h"

void lasers_init() {
	sbi(DDRD,PORTD3);	//définie la sortie B du timer2
	//OCR2A=0x78;		//120//défini la fréquence de 16,67kHz en accord avec le prescaler
	//OCR2B=0x3C;		//60//défini le rapport cyclique de 50%
	//OCR2A=72;
	//OCR2B=36;
	//OCR2A=50;
	//OCR2B=25;
	OCR2A=24;
	OCR2B=12;
	/*positionner le mode pwm*/
	sbi(TCCR2A,WGM20);	//
	sbi(TCCR2A,WGM21);	//fast-pwm, TOP=OCR2A
	sbi(TCCR2B,WGM22);	//
	/*paramétrer le comportement sur comparaisons*/
	//sbi(TCCR2A,COM2B1);	//fast-pwm -> OC2B mis à 0 lors d'une comparaison
	//cbi(TCCR2A,COM2B0);	//réussie et mis à 1 à passage par BOTTOM
	/*sélection de la source du timer*/
	cbi(TCCR2B,CS22);	//
	sbi(TCCR2B,CS21);	//prescaler à 8
	cbi(TCCR2B,CS20);	//
	return ;
}

void lasers_start() {
	sbi(TCCR2A,COM2B1);	//fast-pwm -> OC2B mis à 0 lors d'une comparaison
	cbi(TCCR2A,COM2B0);	//réussie et mis à 1 à passage par BOTTOM
}

void lasers_stop() {
	cbi(TCCR2A,COM2B1);	//pas de pwm, OC2B est déconnecté
	cbi(TCCR2A,COM2B0);	//
}
