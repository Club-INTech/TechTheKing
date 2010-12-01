#include <avr/interrupt.h>
#include <avr/io.h>

#include <HardwareSerial.h>
#include <wiring.h>

#include "Manager.h"

#define ABS(x) 		((x) < 0 ? - (x) : (x))

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

void
Manager::assPolaire()
{

/* Calculs de position du robot
*
*/
	long int angle = 	encodeurG - encodeurD;
	long int distance = 	encodeurG + encodeurD;

/*
*Réactualisation des vitesses du robot
*/

	assRotation.setVitesse((angle-angleBkp)*1000); // 305 = 1000/(3.279ms)  pour avoir la vitesse en tic/s
	assTranslation.setVitesse((distance-distanceBkp)*1000); // Meme chose
	angleBkp = angle;
	distanceBkp = distance;

/*
* On changera de consigne si :
*	-on est suffisament proche de la consigne en distance
*	-on est suffisament aligné avec la consigne
* Ceci ne s'applique pas à la dernière consigne
*/


/*
* factorisation de la désactivation de Kd
*/
if( indiceConsigneActuelle < tableauConsignes.nbConsignes ) {
	assRotation.setActivationKd(0);
	assTranslation.setActivationKd(0);
}
else{
	assRotation.setActivationKd(1);
	assTranslation.setActivationKd(1);
}
	

if(ABS((tableauConsignes.listeConsignes[indiceConsigneActuelle-1]).distance - distance) < 60
	&& ABS((tableauConsignes.listeConsignes[indiceConsigneActuelle-1]).angle - angle) < 60 ){
		if( indiceConsigneActuelle < tableauConsignes.nbConsignes ){
		indiceConsigneActuelle++;
	}
}

/*
*Calcul des PWM
*/
	int pwmRotation = (activationAssAngle?assRotation.calculePwm(((tableauConsignes.listeConsignes)[indiceConsigneActuelle-1]).angle,angle):0);
	int pwmTranslation = (activationAssDistance?assTranslation.calculePwm(((tableauConsignes.listeConsignes)[indiceConsigneActuelle-1]).distance,distance):0);

	int pwmG = pwmTranslation + pwmRotation;
	int pwmD = pwmTranslation - pwmRotation;
	

/*
*Envoi des PWM
*/	


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
* Fin de la boucle
*/

		
	/*
	* Reprise TechTheWave
	*/
	 
	// Réactivation des interruptions
	// Reprise de l'envoi de position
	//TIMSK2 |= savTIMSK2; 
}

/*
* Initialisation des pins
*/
Manager::Manager(){
}


void Manager::init()
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
	* Initialisation du timer de l'asservissement @ 78.125 KHz
	* C'est un timer 8bit donc la fréquence de l'asservissement
	* est 78.125/256 Khz = 305Hz soit environ un asservissement toutes les 3.279ms
	*/
	TCCR2A &= ~(1 << CS22);
	TCCR2A |= (1 << CS21);
	TCCR2A &= ~(1 << CS20);
	TCCR2A &= ~(1 << WGM21) & (1 << WGM20);
	TIMSK2 |= (1 << TOIE2);
	TIMSK2 &= ~(1 << OCIE2A);
	
	tableauConsignes.nbConsignes=0;
	indiceConsigneActuelle=1;

	assRotation.changeKp(10);
	assRotation.changePWM(1023);
	assRotation.changeKd(50);
	assRotation.changeKi(0);
	assRotation.changeVmax(0);
	assRotation.changeKpVitesse(0);

	assTranslation.changeKp(15);
	assTranslation.changePWM(1023);
	assTranslation.changeKd(50);
	assTranslation.changeKi(0);
	assTranslation.changeVmax(0);
	assTranslation.changeKpVitesse(0);	

	distanceTotale=0;
	angleTotal=0;
	angleBkp=0;
	distanceBkp=0;

}

/*
* Fonctions de test : courbe de B�zier. (voir PC/Lib/Bezier)
*/

void	Manager::test(){
	cli();	
	unsigned int i;
	tableauConsignes.nbConsignes=50;
	changeIemeConsigne( 32, 235,1);
	changeIemeConsigne( 98, 461,2);
	changeIemeConsigne( 167, 678,3);
	changeIemeConsigne( 238, 887,4);
	changeIemeConsigne( 313, 1087,5);
	changeIemeConsigne( 391, 1280,6);
	changeIemeConsigne( 471, 1466,7);
	changeIemeConsigne( 555, 1645,8);
	changeIemeConsigne( 642, 1817,9);
	changeIemeConsigne( 733, 1983,10);
	changeIemeConsigne( 826, 2144,11);
	changeIemeConsigne( 923, 2299,12);
	changeIemeConsigne( 1023, 2449,13);
	changeIemeConsigne( 1127, 2594,14);
	changeIemeConsigne( 1233, 2735,15);
	changeIemeConsigne( 1343, 2872,16);
	changeIemeConsigne( 1456, 3006,17);
	changeIemeConsigne( 1571, 3137,18);
	changeIemeConsigne( 1689, 3265,19);
	changeIemeConsigne( 1809, 3391,20);
	changeIemeConsigne( 1932, 3515,21);
	changeIemeConsigne( 2056, 3637,22);
	changeIemeConsigne( 2181, 3758,23);
	changeIemeConsigne( 2308, 3879,24);
	changeIemeConsigne( 2435, 3999,25);
	changeIemeConsigne( 2562, 4119,26);
	changeIemeConsigne( 2689, 4239,27);
	changeIemeConsigne( 2816, 4361,28);
	changeIemeConsigne(2941, 4483,29);
	changeIemeConsigne( 3065, 4607,30);
	changeIemeConsigne( 3188, 4733,31);
	changeIemeConsigne( 3308, 4861,32);
	changeIemeConsigne( 3426, 4991,33);
	changeIemeConsigne( 3542, 5125,34);
	changeIemeConsigne( 3654, 5263,35);
	changeIemeConsigne( 3764, 5404,36);
	changeIemeConsigne( 3871, 5549,37);
	changeIemeConsigne( 3974, 5699,38);
	changeIemeConsigne(4074, 5854,39);
	changeIemeConsigne(4171, 6014,40);
	changeIemeConsigne(4265, 6181,41);
	changeIemeConsigne(4355, 6353,42);
	changeIemeConsigne(4442, 6532,43);
	changeIemeConsigne(4526, 6717,44);
	changeIemeConsigne(4607, 6910,45);
	changeIemeConsigne(4684, 7111,46);
	changeIemeConsigne(4759, 7320,47);
	changeIemeConsigne(4831, 7537,48);
	changeIemeConsigne(4900, 7763,49);
	changeIemeConsigne(4966, 7998,50);
	sei();

}

void
Manager::changeIemeConsigne(long int distanceDonnee, long int angleDonne,int i)
{
	(tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
	(tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
}


/*
* Fonctions utiles au transfert de la liste de points via la liaison série.
*/


void
Manager::setNbConsignes(int nbConsignesDonne)
{
	tableauConsignes.nbConsignes=nbConsignesDonne;
}


void 
Manager::changeIemeConsigneDistance(long int distanceDonnee, int i)
{
	(tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
}

void
Manager::changeIemeConsigneAngle(long int angleDonne, int i)
{
	(tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
}

/*
*A voir, si on peut envoyer via un long int à la fois la distance et l'angle.
*Diviserait par environ deux le temp de chargement de la liste de points en série.
*/


void 
Manager::pushConsigneDistance(long int distanceDonnee) // on transfert d'abord la distance (pas d'incr�mentation de nbConsignes)
{
	tableauConsignes.nbConsignes+=1; //ajout d'une case dans le tableau.
	changeIemeConsigneDistance(distanceDonnee, (tableauConsignes.nbConsignes));
}


void 
Manager::pushConsigneAngle(long int angleDonne)
{
	changeIemeConsigneAngle(angleDonne, (tableauConsignes.nbConsignes) );

}


/*
* Change les asservissements d'état
*/


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

/*
* reset l'asservissement
*/

void Manager::reset()
{
	cli();
	
	Serial.print((tableauConsignes.listeConsignes[indiceConsigneActuelle-1]).distance);
	Serial.print(" ");
	Serial.print((tableauConsignes.listeConsignes[indiceConsigneActuelle-1]).angle);
	Serial.print("   ");
	Serial.print(tableauConsignes.nbConsignes);
	Serial.print("\n");
	encodeurG=0;
	encodeurD=0;
	distanceBkp = 0;
	angleBkp = 0;
	indiceConsigneActuelle=1;
	tableauConsignes.nbConsignes=0;
	(tableauConsignes.listeConsignes[0]).distance = 0;
	(tableauConsignes.listeConsignes[0]).angle = 0;
	sei();
}



/*
* Portion à modifier 
* Réduire le préscaler à 500Hz
*/
 

/*
* Comprends pas.
*/

unsigned char stator1 = 1;

ISR(TIMER2_OVF_vect)
{
	manager.assPolaire();
}


volatile long int encodeurG = 0;
volatile long int encodeurD = 0;

Manager manager;