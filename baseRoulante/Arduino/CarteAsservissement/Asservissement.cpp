#include "Asservissement.h"

#define ABS(x) 		((x) < 0 ? - (x) : (x))
#define MAX(a,b) 	((a) > (b) ? (a) : (b)) 
#define MIN(a,b) 	((a) < (b) ? (a) : (b)) 
#define DECEL 32768

//#define NO_ACC

Asservissement::Asservissement()
{
	// Constante de l'asservissement et du mouvement
	maxPWM = 	PUISSANCE;
	Kp = 		KP;
	Vmax = 		VMAX;
	Kd = 		KD;
	
	// Consigne par défaut et position du robot à l'initialisation
	consigne = 0;
	integraleErreur=0;

		

	
	// Aucun blocage à l'initialisation
	blocageDetecte = 0;
	blocageTemp = 0;
	
	erreur = 0;
	erreurBkp = 0;

	
	// Calcul de l'erreur maximum afin de détecter les blocages
	calculeErreurMax();
}

/*
 * Calcule la puissance moteur à fournir pour atteindre la nouvelle position théorique
 */
 
int 
Asservissement::calculePwm(long int positionReelle)
{
	long int erreur = (consigne - positionReelle);
	integraleErreur+=erreur;
	long int pwm = Kp * erreur + Kd/100  * (erreurBkp-erreur) + Ki/100 * integraleErreur; // Le facteur 256(freq des overflow) est inclu dans Kd et Ki pour moins de calcul

	erreurBkp = erreur;
	
	if (pwm > maxPWM) {
		pwm = maxPWM;
	}
	else if (pwm < -maxPWM ) {
		pwm = -maxPWM;
	}
	
	return pwm;
}



/*
 * Calcule l'erreur maximum (positionReelle et positionIntermediaire) afin de déterminer le blocage
 */

void 
Asservissement::calculeErreurMax()
{
	erreurMax = (PRESCALER * 2 * maxPWM) / Kp;  
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
}

/*
 * Définition dynamique des constantes
 */
void 
Asservissement::changeKp(unsigned int KpDonne)
{
	Kp = KpDonne;
	calculeErreurMax();
}


void 
Asservissement::changePWM(int maxPwmDonne)
{
	maxPWM = maxPwmDonne;
	calculeErreurMax();
}


void
Asservissement::changeVmax(long int VmaxDonne)
{
	Vmax = VmaxDonne;
}

void
Asservissement::changeKd(unsigned int KdDonne)
{
	Kd = KdDonne;
}

void
Asservissement::changeKi(unsigned int KiDonne)
{
	Ki = KiDonne;
}


void
Asservissement::reset() 
{
	consigne = 0;
	
	
	blocageDetecte = 0;
}

