#ifndef Asservissement_h
#define Asservissement_h

// Puissance max. de l'asservissement comprise entre 0 et 1024
#define	PUISSANCE	1024

// Constante de l'asservissement
#define KP		30
#define VMAX		50000
#define ACC		22
#define KD		35

#define PRESCALER	128

#define TRIGGER_BLOCAGE	15

class Asservissement{
	public:
		Asservissement();
		
		void	changeConsigne(long int);
		
		
		void 	stop();
		void 	stopUrgence(long int); 
		
		int	calculePwm(long int);

		void	changeKp(unsigned int);
		void	changeKd(unsigned int);
		void	changeKi(unsigned int);
		void	changeKpVitesse(unsigned int);

		void 	changeAcc(long int);
		void	changeVmax(long int);
		void 	changePWM(int);

		void	reset();
		
		void	setVitesse(long int);	
	
		// Consigne et position du robot (point de vue Arduino)
		long int 	consigne;	
		

		// Constantes de l'asservissement et du moteur	
		long int 	kp; 
		long int	kd;
		long int	ki;
		long int	kpVitesse;

		long int 	vMax;
		long int 	maxPWM; 
		

		// erreur
		long int	erreur;
		long int	erreurBkp;
		long int	integraleErreur;


		// Vaut 1 ou -1 si le moteur est bloqué
		int 		blocageDetecte;
		int		blocageTemp;

		// Vitesse du robot;
		long int	vitesse;

		
};
		
		
#endif
