/**
 * \file Asservissement.h
 * \brief header de Asservissement.cpp
 */
 
#ifndef Asservissement_h
#define Asservissement_h

// Puissance max. de l'asservissement comprise entre 0 et 1024
#define	PUISSANCE	300

// Constante de l'asservissement
#define KP		30
#define VMAX		30000
#define ACC		22
#define KD		35

#define PRESCALER	128

#define TRIGGER_BLOCAGE	15

/**
 * \brief Se charge des fonctions d'asservissement
 */
class Asservissement{
	public:
		Asservissement(); 
		
		void changeConsigne(long int);

		int calculePwm(long int positionReelle);
		void 	calculePositionIntermediaire(long int);
		
		void stop();
		void stopUrgence(long int); 
		
		void calculeErreurMax();
		
		void changeKp(int);
		void changeAcc(long int);
		void changeVmax(long int);
		void changePWM(int);
		void changeKd(long int);
		
		void reset();
		
		// Consigne et position du robot
		long int consigne; //!< consigne en position
		long int positionIntermediaire; //!< la prochaine position que l'on atteind
		
		// Consigne et position du robot zoomé
		long int consigneZoom; //!< consigne zoomée
		long int positionIntermediaireZoom; //!< la prochaine position zoomée que l'on atteind

		// Constantes de l'asservissement et du moteur	
		long int Kp; //!< le coefficient proportiel
		long int Kd; //!< le coefficient dérivateur
		long int deltaBkp; //!< je sais pas ce que c'est
		long int Acc; //!< l'accélération max
		long int Vmax; //!< la vitesse max
		long int maxPWM; //!< le PWM max
		long int dFreinage; //!< la distance de freinage
		long int n; //!< le palier de vitesse
		long int erreurMax; //!< l'erreur maximum avant de dire que l'on est bloqué
		long int erreur; //!< l'erreur courante du robot
		int blocageDetecte; //!< Vaut 1 ou -1 si le moteur est bloqué
		int blocageTemp; //!< le sais pas ce que c'est
};

#endif
