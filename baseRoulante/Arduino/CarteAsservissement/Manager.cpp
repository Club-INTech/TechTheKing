#include <avr/interrupt.h>
#include <avr/io.h>

#include <HardwareSerial.h>
#include <wiring.h>

#include "Manager.h"

/*
 * Fonction d'interruption sur les codeurs
 */

volatile char etatPins = 0;

ISR(PCINT2_vect)
{
    	unsigned char changementPins 	= (PIND & MASQUE) ^ etatPins;
    	etatPins 		= PIND & MASQUE;
    
    	if (changementPins & ENCGA){
		if (etatPins & ENCGA) //front montant codeur 1 voix A
			(etatPins & ENCGB)?encodeurG++:encodeurG--;
	      	else                  //front descendant codeur 1 voix A
			(etatPins & ENCGB)?encodeurG--:encodeurG++;
	}

    	if (changementPins & ENCGB){
		if (etatPins & ENCGB) //front montant codeur 1 voix B
			(etatPins & ENCGA)?encodeurG--:encodeurG++;
		else                  //front descendant codeur 1 voix B
			(etatPins & ENCGA)?encodeurG++:encodeurG--;
	}
    
	if (changementPins & ENCDA){
		if (etatPins & ENCDA) //front montant codeur 2 voix A
			(etatPins & ENCDB)?encodeurD++:encodeurD--;
		else                  //front descendant codeur 2 voix A
			(etatPins & ENCDB)?encodeurD--:encodeurD++;
	}
 
	if (changementPins & ENCDB){
		if (etatPins & ENCDB) //front montant codeur 2 voix B
			(etatPins & ENCDA)?encodeurD--:encodeurD++;
	        else                  //front descendant codeur 2 voix B
			(etatPins & ENCDA)?encodeurD++:encodeurD--;
	}

}
/*
 * Fonction à exécuter à intervalle de temps régulier (~1kHz)
 * Ajouter un timer, priorité sur les interruptions
 */
void 
Manager::assPolaire()
{
	//static int stator1337 = 100;
	
	long int angle = 	encodeurG - encodeurD;
	long int distance = 	encodeurG + encodeurD;
	/*
	 * Reprise TechTheWave, priorité des interruptions
	 */
	 
	// Sauvegarde de TIMSK2
	//unsigned char savTIMSK2 = TIMSK2; // un unsigned fera l'affaire ! -- Yann Sionneau
	// Asservissement prioritaire sur l'envoi de la position
	//TIMSK2 &= ~(1<<TOIE2); 
	// Réactivation des interruptions pour les encodeurs
	sei();  


	
		// Activation de l'asservissement
	if(typeAsservissement==0){
	int pwmRotation = (activationAssAngle?assRotation.calculePwmPosition(angle):0);
	int pwmTranslation = (activationAssDistance?assTranslation.calculePwmPosition(distance):0);
	}
	else{
	int pwmRotation = (activationAssVitRot?assVitesseRotation.calculePwm((angle-angleBkp)/256):0);
	int pwmTranslation = (activationAssDistance?assVitesseTranslation.calculePwmVitesse((distance-distanceBkp)/256):0);
	}

	int pwmG = pwmTranslation + pwmRotation;
	int pwmD = pwmTranslation - pwmRotation;
	
	angleBkp = angle;
	distanceBkp = distance;
	
	// Coefficient trouvé de manière empirique

	if (pwmG > PWM_MAX) 
		pwmG = PWM_MAX;
	else if (pwmG < -PWM_MAX) 
		pwmG = -PWM_MAX;

	if (pwmD > PWM_MAX) 
		pwmD = PWM_MAX;
	else if (pwmD < -PWM_MAX) 
		pwmD = -PWM_MAX;
	
	if (pwmG > 0) {
		// Direction gauche = 0
		// PWM gauche = pwmG
		PORTB &= ~PINDIRG;
		OCR1A = pwmG;
	}
	else {
		// Direction gauche = 1
		// PWM gauche = -pwmG
		PORTB |= PINDIRG; 
		OCR1A = -pwmG;
	}

	if (pwmD > 0) {
		// Direction droite = 0
		// PWM droite = pwmD
		PORTB &= ~PINDIRD;
		OCR1B = pwmD;
	}
	else {
		// Direction droite = 1
		// PWM droite = -pwmD
		PORTB |= PINDIRD;
        	OCR1B = -pwmD;
	}
		
	/*
	 * Reprise TechTheWave
	 */
	 
	// Réactivation des interruptions
	//cli();
	// Reprise de l'envoi de position
	//TIMSK2 |= savTIMSK2; 
}

/*
 * Initialisation des pins
 */
Manager::Manager() 
{
}

void
Manager::init()
{
	activationAssDistance = true;
	activationAssAngle = true;
	
	/*
	 * Réglage des pins (codeurs)
	 */
	pinMode(PORTB2, INPUT);
	pinMode(PORTB3, INPUT);
	pinMode(PORTB4, INPUT);
	pinMode(PORTB5, INPUT);
	
	digitalWrite(PORTB2, HIGH);
	digitalWrite(PORTB3, HIGH);
	digitalWrite(PORTB4, HIGH);
	digitalWrite(PORTB5, HIGH);

	// Initialisation de l'interruption
	PCICR |= (1 << PCIE2);
  	PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21);
	
	/*
	 * Réglage des PWM
	 */

	pinMode(DIRG, OUTPUT);
	pinMode(PWMG, OUTPUT);
	
	pinMode(DIRD, OUTPUT);
	pinMode(PWMD, OUTPUT);
	
	digitalWrite(PWMG, 0);
	digitalWrite(PWMD, 0);
	
	digitalWrite(DIRG, 0);
	digitalWrite(DIRD, 0);

	// Fast PWM
	TCCR1A = (1 << WGM11) | (1 << WGM10);
	TCCR1B = (1<<CS10); 			// Divise la fréq du timer par1
	TCCR1A |= (1 << COM1A1); 		// Timer de base, 2 ports spéciaux pour les créneaux
	TCCR1A |= (1 << COM1B1);
	 
	/*
	 * Initialisation du timer de l'asservissement @ 1kHz
	 */
	TCCR2A |= (1 << CS22) | (0 << CS21) | (0 << CS20);
	TCCR2A |= (0 << WGM21) |(0 << WGM20);
	TIMSK2 |= (1 << TOIE2) | (0 << OCIE2A);
	
	assRotation.changeKp(25);
	assRotation.changePWM(1024);
	assRotation.changeKd(375);
        assRotation.changeKi(0);

	assTranslation.changeKp(30);
	assTranslation.changePWM(1024);
	assTranslation.changeKd(300);
	assTranslation.changeKi(0);
	
	assVitesseRotation.changeKp(25);
	assVitesseRotation.changePWM(1024);
	assVitesseRotation.changeKd(375);
        assVitesseRotation.changeKi(0);

	assVitesseTranslation.changeKp(25);
	assVitesseTranslation.changePWM(1024);
	assVitesseTranslation.changeKd(375);
        assVitesseTranslation.changeKi(0);


	typeAsservissement=0;//0 : assPosition, 1 : assVitesse
}

/*
 * Définit la nouvelle consigne en angle et distance (en tics)
 */
void 
Manager::changeConsigne(long int distanceDonnee, long int angleDonne)
{
	assTranslation.changeConsigne(distanceDonnee);
	assRotation.changeConsigne(angleDonne);
}

void
Manager::changeConsigneAngle(long int angleDonne)
{
	assRotation.changeConsigne(angleDonne);
}

void 
Manager::changeConsigneTranslation(long int distanceDonnee)
{
	assTranslation.changeConsigne(distanceDonnee);
}


void
Manager::changeConsigneVitesseRot(long int vitesseRotDonnee)
{
	assVitesseRotation.changeConsigne(vitesseRotDonnee);
}



void 
Manager::changeConsigneDistance(long int vitesseTransDonnee)
{
	assVitesseTranslation.changeConsigne(vitesseTransDonnee);
}

void 
Manager::switchAssDistance()
{
	activationAssDistance = !activationAssDistance;
}

void 
Manager::switchAssAngle()
{
	activationAssAngle = !activationAssAngle;
}

void

void
Manager::switchTypeAsservissement()
{
	typeAsservissement = !typeAsservissement;
}

Manager::reset()
{
	cli();
	encodeurG = 0;
	encodeurD = 0;
	assRotation.reset();
	assTranslation.reset();
	sei();
}
/*
 * Portion à modifier 
 * Réduire le préscaler à 500Hz
 */
 
unsigned char stator1 = 1;

ISR(TIMER2_OVF_vect)
{
	if (stator1 == 0) {
		manager.assPolaire();
		stator1 = 1;
	}
	else {
		stator1--;
	}
}


volatile long int encodeurG = 0;
volatile long int encodeurD = 0;

Manager manager;

