#include "Asservissement.h"

#define ABS(x) 		((x) < 0 ? - (x) : (x))
#define MAX(a,b) 	((a) > (b) ? (a) : (b)) 
#define MIN(a,b) 	((a) < (b) ? (a) : (b)) 

//#define NO_ACC

Asservissement::Asservissement()
{
	// Constante de l'asservissement et du mouvement
	maxPWM = 	0;
	kp = 		0;
	vMax = 		0;
	kd = 		0;
	ki =		0;
	kpVitesse =	0;
	
	// Consigne par défaut et position du robot à l'initialisation
	consigne = 0;
	integraleErreur=0;

	// Vitesse du robot
	vitesse = 0;

		

	
	// Aucun blocage à l'initialisation
	blocageDetecte = 0;
	blocageTemp = 0;
	
	erreur = 0;
	erreurBkp = 0;

}


/*
 * Calcule la puissance moteur à fournir pour atteindre la nouvelle position théorique
 */
int Asservissement::calculePwm(long int positionReelle)
{
	long int erreur = (consigne - positionReelle);
	if(erreur<=3)
		integraleErreur=0;
	else
		integraleErreur+=erreur;
	long int pwm = kp * erreur - kd * vitesse / 100 - ki  * integraleErreur; // la dérivée de l'erreur est égale à -vitesse . On divise par 1000 car sinon kd > 1

	if(vitesse>vMax){
		pwm+=kpVitesse*(vMax-vitesse); // pas besoin de dérivateur ou d'intégrateur ici
	}

	if (pwm > maxPWM){
		pwm = maxPWM;
	}
	else if (pwm < -maxPWM ) 
	{
		pwm = -maxPWM;
	}
	
	return pwm;
}




/*
 * Arrêt progressif du moteur
 */
 
void 
Asservissement::stop()
{

}

/*
 * Arrete le moteur à la position courante
 */
 
void 
Asservissement::stopUrgence(long int positionReelle)
{
	changeConsigne(positionReelle);
}


/*
 * Définit la nouvelle consigne
 */
 
void 
Asservissement::changeConsigne(long int consigneDonnee)
{
	consigne = consigneDonnee;
	integraleErreur = 0;
}

/*
 * Définition dynamique des constantes
 */
void 
Asservissement::changeKp(unsigned int kpDonne)
{
	kp = kpDonne;
}


void 
Asservissement::changePWM(int maxPwmDonne)
{
	maxPWM = maxPwmDonne;
}


void
Asservissement::changeVmax(long int vMaxDonne)
{
	vMax = vMaxDonne;
}

void
Asservissement::changeKd(unsigned int kdDonne)
{
	kd = kdDonne;
}

void
Asservissement::changeKi(unsigned int kiDonne)
{
	ki = kiDonne;
}


void
Asservissement::changeKpVitesse(unsigned int kpVitesseDonne)
{
	kpVitesse=kpVitesseDonne;
}

void
Asservissement::setVitesse(long int vitesseDonnee)
{
	vitesse = vitesseDonnee;
}

void
Asservissement::reset() 
{
	consigne = 0;
	blocageDetecte = 0;}
