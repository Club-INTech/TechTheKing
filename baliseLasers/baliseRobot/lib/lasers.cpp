#include "lasers.h"


/* le mode 16 est le plus joli */

#define NM_SIN 16
#define NM_SIN_1 15
uint8_t sin_lasers[NM_SIN] = {
	128,
	176,
	217,
	245,
	255,
	245,
	217,
	176,
	128,
	79,
	38,
	10,
	1,
	10,
	38,
	79};

/* mais le mode 8 est plus rapide */

/*#define NM_SIN 8
#define NM_SIN_1 7
uint8_t sin_lasers[NM_SIN] = {
	128,
	217,
	255,
	217,
	128,
	38,
	1,
	38};*/

volatile uint8_t i=0;

void lasers_init() {
	sbi(DDRB,PORTB3);	//définie la sortie A du timer2
	/* positionner le mode pwm */
	sbi(TCCR2A,WGM20);	//
	sbi(TCCR2A,WGM21);	//fast-pwm, TOP=OCR2A
	cbi(TCCR2B,WGM22);	//
	/* paramétrer le comportement sur comparaisons */
	sbi(TCCR2A,COM2A1);	//fast-pwm -> OC2A mis à 0 lors d'une comparaison
	cbi(TCCR2A,COM2A0);	//réussie et mis à 1 à passage par BOTTOM
	/* sélection de la source du timer */
	cbi(TCCR2B,CS22);	//
	cbi(TCCR2B,CS21);	//pas de prescaler
	sbi(TCCR2B,CS20);	//
	/* Activation des interruptions sur overflow */
	//sbi(TIMSK2,TOIE2);    // réservé pour la fonction start
	OCR2A=0;
	return ;
}

void lasers_start() {
	/* Activation des interruptions sur overflow */
	sbi(TIMSK2,TOIE2);
	OCR2A=0;
}

void lasers_stop() {
	/* On coupe la variation de OCR2A */
	cbi(TIMSK2,TOIE2);
	/* Et on le fixe à 0 */
	OCR2A=0;
}

ISR(TIMER2_OVF_vect) {
	OCR2A=sin_lasers[i];
	if ( i<(NM_SIN_1) ) {
		i++;
	}
	else {
		i=0;
	}
}
