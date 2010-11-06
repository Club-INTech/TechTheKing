#include "Asservissement.h"

#define ABS(x) 		((x) < 0 ? - (x) : (x))
#define MAX(a,b) 	((a) > (b) ? (a) : (b)) 
#define MIN(a,b) 	((a) < (b) ? (a) : (b)) 

//#define NO_ACC

Asservissement::Asservissement()
{
	// Constante de l'asservissement et du mouvement
	maxPWM = 	0;
	Kp = 		0;
	Vmax = 		0;
	Kd = 		0;
	Ki =		0;
	
	// Consigne par défaut et position du robot à l'initialisation
	consigne = 0;
	integraleErreur=0;
	acc=3;

		

	
	// Aucun blocage à l'initialisation
	blocageDetecte = 0;
	blocageTemp = 0;
	
	erreur = 0;
	erreurBkp = 0;

}

AsservissementVitesse::AsservissementVitesse(){
	pwm=0;
}

/*
 * Calcule la puissance moteur à fournir pour atteindre la nouvelle position théorique
 */
int AsservissementPosition::calculePwmPosition(long int positionReelle)
{
	long int erreur = (consigne - positionReelle);
	if(ABS(erreur)<=4){
		integraleErreur=0;
		return 0;
	}
	else
	integraleErreur+=erreur;
	long int pwm = (Kp * erreur + Kd  * (erreur - erreurBkp) + Ki  * integraleErreur); // Le facteur 256(freq des overflow) est inclu dans Kd et Ki pour moins de calcul
	erreurBkp = erreur;
	if (pwm > maxPWM){
		pwm = maxPWM;
	}
	else if (pwm < -maxPWM ) 
	{
		pwm = -maxPWM;
	}
	
	return pwm;
}


int AsservissementVitesse::calculePwmVitesse(long int positionReelle)
{
	long int erreur = (consigne - positionReelle);
	if(ABS(erreur)<=4){
		integraleErreur=0;
		return 0;
	}
	else
	integraleErreur+=erreur;
	pwm += (Kp * erreur + Kd  * (erreur - erreurBkp) + Ki  * integraleErreur); // Le facteur 256(freq des overflow) est inclu dans Kd et Ki pour moins de calcul
	erreurBkp = erreur;
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
	integraleErreur = 0;
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
	blocageDetecte = 0;}
