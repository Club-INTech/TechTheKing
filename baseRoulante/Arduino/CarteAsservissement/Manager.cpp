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
	long int angleAvantChangement = 	encodeurG - encodeurD;
	long int distanceAvantChangement = 	encodeurG + encodeurD;

/*
*Réactualisation des vitesses du robot
*/

	assRotation.setVitesse((angleAvantChangement-angleBkp)*305); // 305 = 1000/(3.279ms)  pour avoir la vitesse en tic/s
	assTranslation.setVitesse((distanceAvantChangement-distanceBkp)*305); // Meme chose
	angleBkp = angleAvantChangement;
	distanceBkp = distanceAvantChangement;

/*
* On changera de consigne si :
*	-on est suffisament proche de la consigne en distance
*	-on est suffisament aligné avec la consigne
* Ceci ne s'applique pas à la dernière consigne
*/

/*
* factorisation de la désactivation de Kd
*/

assRotation.setActivationKd(0);
assTranslation.setActivationKd(0);


	if(  ABS(distanceAvantChangement) > ABS((tableauConsignes.listeConsignes[indiceConsigneActuelle-1]).distance) * 0.8 && ABS(angleAvantChangement) > ABS((tableauConsignes.listeConsignes[indiceConsigneActuelle-1]).angle)*0.95)
	{
		if( indiceConsigneActuelle < tableauConsignes.nbConsignes  )
		{
			indiceConsigneActuelle+=1;
			distanceTotale+=encodeurG+encodeurD; // Conservation de l'information présente dans les codeuses au changement de consigne.
			angleTotal+=encodeurG-encodeurD; // idem

 /* 
  * On reset la position du robot (On considère que le robot ne fait que des segments...)
  */			
			encodeurG=0;
			encodeurD=0;
			angleBkp=0;
			distanceBkp=0;
			
/*
*  Lors du passage à la dernière consigne on réactive Kd
*/
			if(indiceConsigneActuelle=tableauConsignes.nbConsignes{
				assRotation.setActivationKd(1);
				assTranslation.setActivationKd(1);
			}
		}
	}

/*
*Calcul des PWM
*/
	
	int pwmRotation = (activationAssAngle?assRotation.calculePwm(((tableauConsignes.listeConsignes)[indiceConsigneActuelle-1]).angle,angleAvantChangement):0);
	int pwmTranslation = (activationAssDistance?assTranslation.calculePwm(((tableauConsignes.listeConsignes)[indiceConsigneActuelle-1]).distance,distanceAvantChangement):0);

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
	TCCR2A |= (1 << CS22);
	TCCR2A &= ~(1 << CS21);
	TCCR2A |= ~(1 << CS20);
	TCCR2A &= ~(1 << WGM21) & (1 << WGM20);
	TIMSK2 |= (1 << TOIE2);
	TIMSK2 &= ~(1 << OCIE2A);
	
	tableauConsignes.nbConsignes=1;
	indiceConsigneActuelle=1;

	assRotation.changeKp(20);
	assRotation.changePWM(1023);
	assRotation.changeKd(50);
        assRotation.changeKi(0);
        assRotation.changeVmax(0);
	assRotation.changeKpVitesse(0);

	assTranslation.changeKp(20);
	assTranslation.changePWM(1024);
	assTranslation.changeKd(50);
	assTranslation.changeKi(0);
        assTranslation.changeVmax(0);
	assTranslation.changeKpVitesse(0);	

	angleBkp=0;
	distanceBkp=0;

}

/*
 * Ajoute une consigne à la liste.
 */

void 
Manager::pushConsigne(long int distanceDonnee, long int angleDonne)
{
	tableauConsignes.nbConsignes+=1; //ajout d'une case dans le tableau.
	changeIemeConsigne(distanceDonnee, angleDonne, (tableauConsignes.nbConsignes) );

}

/*
 * Change une consigne de la liste
 */


void 
Manager::changeIemeConsigne(long int distanceDonnee, long int angleDonne,int i)
{
	(tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
	(tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
}

void
Manager::changeIemeConsigneAngle(long int angleDonne, int i)
{
	(tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
	(tableauConsignes.listeConsignes[i-1]).distance=0;
}


void 
Manager::changeIemeConsigneDistance(long int distanceDonnee, int i)
{
	(tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
	(tableauConsignes.listeConsignes[i-1]).angle=0;
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
	tableauConsignes.nbConsignes=1;
	indiceConsigneActuelle=1;
	encodeurG = 0;
	encodeurD = 0;
	distanceBkp = 0;
	angleBkp = 0;
	(tableauConsignes.listeConsignes[0]).distance = 0;
	(tableauConsignes.listeConsignes[0]).angle = 0;
	sei();
}

/*
* Fait faire une courbe au robot(test)
*/

void	Manager::test(){
	cli();	
	unsigned int i;
	tableauConsignes.nbConsignes=0;
	for(i=1;i<=10;i++)
		manager.pushConsigne( 0 , 200); //avance sans tourner de 2000
	for(i=1;i<=20;i++)
		manager.pushConsigne( 50 , 100); //avance régulièrement en tournant
	indiceConsigneActuelle=1;
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

