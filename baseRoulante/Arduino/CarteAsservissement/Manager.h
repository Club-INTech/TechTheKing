#ifndef Manager_h
#define Manager_h

#include <avr/interrupt.h>
#include <avr/io.h>

#include "Asservissement.h"

// Puissance maximal de chaque moteur (1023 MAX)
#define PWM_MAX	1023

/*
 * Réglage des masques des codeurs
 * On utilise PORTB2 à 5
 */
#define ENCGA (1 << PORTB2)
#define ENCGB (1 << PORTB4)

#define ENCDA (1 << PORTB3)
#define ENCDB (1 << PORTB5)

#define MASQUE B0111100

#define NB_MAX_CONSIGNES

/*
 * Réglage des pins des PWM
 */

// Roue Gauche
#define DIRG 11
#define PWMG 9

// Roue Droite
#define DIRD 12
#define PWMD 10

#define PINDIRG (1 << PORTB3)
#define PINDIRD (1 << PORTB4)

class Manager {
	public:
		Manager();
		
		void 	init();

		void 	pushConsigne(long int , long int );
		void 	changeIemeConsigne(long int , long int ,int i);		
		void 	changeIemeConsigneDistance (long int,int i);
		void 	changeIemeConsigneAngle (long int,int i); 

		void 	assPolaire();
		
		void 	switchAssDistance();
		void 	switchAssAngle();

		void	test();
		void	reset();

		Asservissement 	assRotation;
		Asservissement 	assTranslation;

		
	
		// Activation de l'asservissement
	
		// Activation de l'asservissement
		bool		activationAssDistance;
		bool		activationAssAngle;

		long int	angleBkp;

		long int	distanceBkp;

		typedef struct {
			long int distance;
			long int angle;
		}Consigne;

		typedef struct {
			unsigned int nbConsignes;
			Consigne listeConsignes[NB_MAX_CONSIGNES] ;
		}TableauConsignes;

		
		TableauConsignes tableauConsignes;
		unsigned int indiceConsigneActuelle;

};

extern volatile long int 	encodeurG;
extern volatile long int 	encodeurD;

extern Manager 			manager;

#endif
