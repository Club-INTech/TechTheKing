/**
 * \file Asservissement.cpp
 * \brief gère les fonctions d'asservissement
 */
 
#include "Asservissement.h"

#define ABS(x) 		((x) < 0 ? - (x) : (x))
#define MAX(a,b) 	((a) > (b) ? (a) : (b)) 
#define MIN(a,b) 	((a) < (b) ? (a) : (b)) 

/**
 * \fn Asservissement::Asservissement()
 * \brief constructeur de Asservissement
 */
Asservissement::Asservissement()
{
	// Constante de l'asservissement et du mouvement
	maxPWM = PUISSANCE;
	Kp = KP;
	Vmax = VMAX;
	Acc = ACC;
	Kd = KD;
	
	// Consigne par défaut et position du robot à l'initialisation
	positionIntermediaire = 0;
	consigne = 0;
	
	positionIntermediaireZoom = 0;
	consigneZoom = 0;
	
	// Palier de vitesse
	n = 0;
	
	// Aucun blocage à l'initialisation
	blocageDetecte = 0;
	blocageTemp = 0;
	deltaBkp = 0;
	
	erreur =  0;
	
	// Calcul de l'erreur maximum afin de détecter les blocages
	calculeErreurMax();
}

/**
 * \fn int Asservissement::calculePwm(long int positionReelle)
 * \brief calcule le Pwn en fonction de notre position
 * \param positionReelle la position du robot dans son repère
 * \return un int
 */
int Asservissement::calculePwm(long int positionReelle)
{
	long int delta = (positionIntermediaire - positionReelle);
	
	long int pwm = Kp * delta + Kd * (delta-deltaBkp);
	
	if (pwm > maxPWM) {
		pwm = maxPWM;
	}
	else if (pwm < -maxPWM ) {
		pwm = -maxPWM;
	}
	deltaBkp = delta;
	
	return pwm;
}

/**
 * \fn void Asservissement::calculePositionIntermediaire(long int positionReelle)
 * \brief calcule la nouvelle position à atteindre
 * \param positionReelle la position du robot dans son repère
 */
void Asservissement::calculePositionIntermediaire(long int positionReelle)
{
	long int delta = consigneZoom - positionIntermediaireZoom;

#ifndef NO_ACC	
	dFreinage = (Acc * (ABS(n) + 1) * (ABS(n) + 2)) / 2;
	if (ABS(delta) >=  dFreinage) {
		if (delta >= 0) {
			n++;
			if (Acc * n >  Vmax) {
				n--;
				if (Acc * n > Vmax)
					n--;
			}
		}
		else {
			n--;
			if (Acc * n < -Vmax) {
				n++;
				if (Acc * n < -Vmax)
					n++;
			}
		}
	}
	else {
		if (n > 0) {
			n--;
		}
		else if (n < 0) {
			n++;
		}
	}

	erreur = positionIntermediaireZoom - positionReelle * PRESCALER;
	//positionIntermediaireZoom += n * Acc;
	if (ABS(erreur) < erreurMax) {
		positionIntermediaireZoom += n * Acc;
		if (blocageTemp > 0) {
			blocageTemp--;
		}
		if (blocageTemp < 0) {
			blocageTemp++;
		}
	}
	else if (erreur >= 0) {	
		positionIntermediaireZoom = positionReelle * PRESCALER + erreurMax;
		if (blocageTemp < TRIGGER_BLOCAGE) {
			blocageTemp++;
		}
	}
	else {
		positionIntermediaireZoom = positionReelle * PRESCALER - erreurMax;
		if (blocageTemp > -TRIGGER_BLOCAGE) {
			blocageTemp--;
		}
	}
#else
	
	if (delta > Vmax)
		positionIntermediaireZoom += Vmax;
	else if (delta < -Vmax)
		positionIntermediaireZoom -= Vmax;
#endif		
	positionIntermediaire = positionIntermediaireZoom / PRESCALER;
}

/**
 * \fn void Asservissement::calculeErreurMax()
 * \brief calcule l'erreur max afin de détecter tout blocage
 */
void Asservissement::calculeErreurMax()
{
	erreurMax = (PRESCALER * 2 * maxPWM) / Kp;  
}

/**
 * \fn void Asservissement::stop()
 * \brief on arrete le robot
 */
void Asservissement::stop()
{
	if (n > 0)
		consigneZoom = positionIntermediaireZoom + dFreinage;
	else
		consigneZoom = positionIntermediaireZoom - dFreinage;	
}

/**
 * \fn void Asservissement::stopUrgence(long int positionReelle)
 * \brief on s'asservit sur place
 * \param positionReelle la position du robot dans son repère
 */
void Asservissement::stopUrgence(long int positionReelle)
{
	changeConsigne(positionReelle);
	n = 0;
}

/**
 * \fn void Asservissement::changeConsigne(long int consigneDonnee)
 * \brief définit la nouvelle consigne
 * \param consigneDonnee la nouvelle consigne
 */
void Asservissement::changeConsigne(long int consigneDonnee)
{
	consigne = consigneDonnee;
	consigneZoom = consigneDonnee * PRESCALER;
}

/**
 * \fn void Asservissement::changeKp(int KpDonne)
 * \brief change le Kp
 * \param KpDonne le nouveau Kp
 */
void Asservissement::changeKp(int KpDonne)
{
	Kp = KpDonne;
	calculeErreurMax();
}

/**
 * \fn void Asservissement::changePWM(int maxPwmDonne)
 * \brief change le PWM
 * \param maxPwmDonne le PWM maximal
 */
void Asservissement::changePWM(int maxPwmDonne)
{
	maxPWM = maxPwmDonne;
	calculeErreurMax();
}

/**
 * \fn void Asservissement::changeAcc(long int AccDonne)
 * \brief change l'accélération max
 * \param AccDonne l'accélération max
 */
void Asservissement::changeAcc(long int AccDonne)
{
	Acc = AccDonne;
}

/**
 * \fn void Asservissement::changeVmax(long int VmaxDonne)
 * \brief change la vitesse max
 * \param VmaxDonne la vitesse max
 */
void Asservissement::changeVmax(long int VmaxDonne)
{
	Vmax = VmaxDonne;
}

/**
 * \fn void Asservissement::changeKd(long int KdDonne)
 * \brief change le Kd
 * \param KdDonne le coefficient Kd de l'asservissement
 */
void Asservissement::changeKd(long int KdDonne)
{
	Kd = KdDonne;
}

/**
 * \fn void Asservissement::reset() 
 * \brief reset l'asservissement, on pert la position
 */
void Asservissement::reset() 
{
	positionIntermediaire = 0;
	consigne = 0;
	
	positionIntermediaireZoom = 0;
	consigneZoom = 0;
	
	n = 0;
	
	blocageDetecte = 0;
}
