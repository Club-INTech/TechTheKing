#include "lasers.h"


/* le mode 16 est le plus joli */

#define NM_SIN 16
#define NM_SIN_1 15
uint8_t sin_lasers[NM_SIN] = {
	50,
	98,
	142,
	180,
	212,
	236,
	250,
	255,
	250,
	236,
	212,
	180,
	142,
	98,
	50,
	0};

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
volatile uint8_t j=0;

void lasers_init() {
	sbi(DDRB,PORTB5);
	sbi(DDRD,PORTD7);
	sbi(DDRD,PORTD6);	//définie la sortie A du timer0
	/* positionner le mode pwm */
	sbi(TCCR0A,WGM00);	//
	sbi(TCCR0A,WGM01);	//fast-pwm, TOP=OCR0A
	cbi(TCCR0B,WGM02);	//
	/* paramétrer le comportement sur comparaisons */
	sbi(TCCR0A,COM0A1);	//fast-pwm -> OC0A mis à 0 lors d'une comparaison
	cbi(TCCR0A,COM0A0);	//réussie et mis à 1 à passage par BOTTOM
	/* sélection de la source du timer */
	cbi(TCCR0B,CS02);	//
	sbi(TCCR0B,CS01);	//pas de prescaler
	cbi(TCCR0B,CS00);	//
	/* Activation des interruptions sur overflow */
	//sbi(TIMSK2,TOIE2);    // réservé pour la fonction start
	OCR0A=0;
	return ;
}

void lasers_start() {
	/* Activation des interruptions sur overflow */
	sbi(TIMSK0,TOIE0);
	OCR0A=0;
}

void lasers_stop() {
	/* On coupe la variation de OCR0A */
	cbi(TIMSK0,TOIE0);
	/* Et on le fixe à 0 */
	OCR0A=0;
}

ISR(TIMER0_OVF_vect) {
	OCR0A=sin_lasers[i];
	if ( i<(NM_SIN_1) ) {
		i++;
	}
	else {
		i=0;
		if (j==0) {
			cbi(PORTB,PORTB5);
			j++;
			sbi(PORTD,PORTD7);
		}
		else {
			sbi(PORTB,PORTB5);
			j=0;
			cbi(PORTD,PORTD7);
		}
	}
}
