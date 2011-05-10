#include "Manager.h"

#define CONVERSION_TIC_ANGLE 48.330379 //(2 * Pi * 65 536) / 8500
#define CONVERSION_TIC_DISTANCE 0.00000158239294 // 3375/(3500 * 65536)
#define ABS(x) (x > 0 ? x : -x)

volatile long x;
volatile long y;

void
Manager::assPolaire(){
	int32_t infos[2];
	get_all(infos);
    int32_t distance = infos[0];
    int32_t angle = infos[1];
    
    
    x += ( (distance - distanceBkp) * fp_cos( CONVERSION_TIC_ANGLE * angle ) );
    y += ( (distance - distanceBkp) * fp_sin( CONVERSION_TIC_ANGLE * angle ) );
	
	printlnLong(x*CONVERSION_TIC_DISTANCE);
	printlnLong(y*CONVERSION_TIC_DISTANCE);

	// R�actualisation des vitesses du robot
	assRotation.setVitesse((angle-angleBkp));
	assTranslation.setVitesse((distance-distanceBkp));
	
	
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
	
	
	if( indiceConsigneActuelle ==0 || indiceConsigneActuelle ==tableauConsignes.nbConsignes ) {
		assRotation.setActivationKd(1);
		assTranslation.setActivationKd(1);
	}
	else{
		assRotation.setActivationKd(0);
		assTranslation.setActivationKd(0);
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
	int16_t pwmRotation = (activationAssAngle?assRotation.calculePwm(((tableauConsignes.listeConsignes)[indiceConsigneActuelle-1]).angle,angle):0);
	int16_t pwmTranslation = (activationAssDistance?assTranslation.calculePwm(((tableauConsignes.listeConsignes)[indiceConsigneActuelle-1]).distance,distance):0);

	int16_t pwmG = pwmTranslation - pwmRotation;
	int16_t pwmD = pwmTranslation + pwmRotation;
	

	
	/*
	* Envoi des PWM
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
		PORTD |= PINDIR1;
		MOTEUR1 = pwmG;
	}
	else {
		// Direction gauche = 1
		// PWM gauche = -pwmG
		PORTD &= ~PINDIR1;
		MOTEUR1 = -pwmG;
	}
	
	
	if (pwmD > 0) {
		// Direction droite = 0
		// PWM droite = pwmD
		PORTB |= PINDIR2;
		MOTEUR2 = pwmD;
	}
	else {
		// Direction droite = 1
		// PWM droite = -pwmD
		PORTB &= ~PINDIR2;
		MOTEUR2 = -pwmD;
	}
}

/*
* Initialisation des pins
*/
Manager::Manager(){
}


void Manager::init()
{
	x=0;
	y=0;
	
	distanceBkp=0;
	angleBkp=0;
	
	activationAssDistance = true;
	activationAssAngle = true;

    // Initialisation PWM pour le PH sur timer0 (moteur 2)
    // Initialisation pin 12
    DDRD |= ( 1 << PORTD6 );
    TCCR0A &= ~( 1 << COM0A0);
    TCCR0A |=  ( 1 << COM0A1 );
    // Fast PWM
    TCCR0A |= ( 1 << WGM00 );
    TCCR0A |= ( 1 << WGM01 );
    TCCR0B &= ~( 1 << WGM02 );
    // Pas de prescaler
    TCCR0B |= ( 1 << CS00 );

    // Initialisation PWM pour le PH sur timer2 (moteur 1)
    // Initialisation pin 6
    DDRD |= ( 1 << PORTD3 );
    TCCR2A &= ~( 1 << COM2B0 );
    TCCR2A |= ( 1 << COM2B1 );
    // Fast PWM
    TCCR2A |= ( 1 << WGM20 );
    TCCR2A |= ( 1 << WGM21 );
    TCCR2B &= ~( 1 << WGM22 );
    // Pas de prescaler
    TCCR2B |= ( 1 << CS20 );

    // Pins de direction des PH
    DDRD |= ( 1 << PORTD4 );
    DDRB |= ( 1 << PORTB0 );

    // Initialisation ADC
    // ADCSRA |= (1 << ADEN);

	// Timer de l'asservissement (16bit, 20 MHz)
	// Penser � changer le #define prescaler en haut du fichier
	TIMSK1 |= (1 << TOIE1);
	//prescaler 8
    TCCR1B |= (1 << CS11);
   //prescaler 64
   //TCCR1B |= (1 << CS11) | (1 << CS10 );
	
	// initialisation de la liste de point
	tableauConsignes.nbConsignes=1;
	indiceConsigneActuelle=1;

	// initialisation des constantes
	assRotation.changeKp(8);
	assRotation.changePWM(PWM_MAX);
	assRotation.changeKd(300);
	assRotation.changeKi(0);
	assRotation.changeVmax(0);
	assRotation.changeKpVitesse(0);

	assTranslation.changeKp(8);
	assTranslation.changePWM(PWM_MAX);
	assTranslation.changeKd(300);
	assTranslation.changeKi(0);
	assTranslation.changeVmax(0);
	assTranslation.changeKpVitesse(0);

	distanceTotale=0;
	angleTotal=0;
	angleBkp=0;
	distanceBkp=0;

}

/*
* Fonctions de test : courbe de B�zier quasi parabolique.
*/

void	Manager::test(){
	cli();	
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
Manager::changeIemeConsigne(int32_t angleDonne, int32_t distanceDonnee,int16_t i)
{
	(tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
	(tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
}


/*
* Fonctions utiles au transfert de la liste de points via la liaison série.
*/
void
Manager::setNbConsignes(int16_t nbConsignesDonne)
{
	tableauConsignes.nbConsignes=nbConsignesDonne;
}

void 
Manager::changeIemeConsigneDistance(int32_t distanceDonnee, int16_t i){
	(tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
}

void
Manager::changeIemeConsigneAngle(int32_t angleDonne, int16_t i)
{
	(tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
}

void
Manager::reset()
{
	tableauConsignes.nbConsignes=1;
	indiceConsigneActuelle=1;
	send_reset();
}
/*
* A voir, si on peut envoyer via un int32_t à la fois la distance et l'angle.
* Diviserait par environ deux le temp de chargement de la liste de points en série.
*/
void 
Manager::pushConsigneDistance(int32_t distanceDonnee) // on transfert d'abord la distance (pas d'incr�mentation de nbConsignes)
{
	tableauConsignes.nbConsignes+=1; //ajout d'une case dans le tableau.
	changeIemeConsigneDistance(distanceDonnee, (tableauConsignes.nbConsignes));
}

void 
Manager::pushConsigneAngle(int32_t angleDonne)
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
Manager::switchAssAngle(){
	activationAssAngle = !activationAssAngle;
}

ISR(TIMER1_OVF_vect, ISR_NOBLOCK){
	manager.assPolaire();
}

Manager manager;
