#include "lasers.h"

void lasers_init() {
	sbi(DDRD,DDD5);		//définie la sortie B du timer0
	OCR0A=0x30;		//défini la fréquence de 333kHz en accord avec le diviseur
	OCR0B=0x18;		//défini le rapport cyclique de 50%
	/*positionner le mode pwm*/
	sbi(TCCR0A,WGM00);	//
	sbi(TCCR0A,WGM01);	//fast-pwm, TOP=OCR0A
	sbi(TCCR0B,WGM02);	//
	/*paramétrer le comportement sur comparaisons*/
	sbi(TCCR0A,COM0B1);	//fast-pwm -> OC1B mis à 0 lors d'une comparaison
	cbi(TCCR0A,COM0B0);	//réussie et mis à 1 à passage par BOTTOM
	/*sélection de la source du timer*/
	cbi(TCCR0B,CS02);	//
	cbi(TCCR0B,CS01);	//diviseur à 1->le timer s'incrémente à 16MHz
	sbi(TCCR0B,CS00);	//
	return ;
}
