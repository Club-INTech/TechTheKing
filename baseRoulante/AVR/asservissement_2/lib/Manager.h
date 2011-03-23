#ifndef Manager_h
#define Manager_h

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h> 
#include "Asservissement.h"
#include "Util.h"
#include "serial.h"

#define ABS(x) 		((x) < 0 ? - (x) : (x))

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

#define NB_MAX_CONSIGNES 200

/*
 * Réglage des pins des PWM
 */

/*
 *  Valeurs de comparaison pour le fast PWM
 *    entre 0 et 255
 */
#define MOTEUR2     OCR0A
#define MOTEUR1     OCR0B

/*
 *  Pins de direction des PH
 */
#define PINDIR1 (1 << PORTD4)
#define PINDIR2 (1 << PORTB0)

class Manager {
	public:
		Manager();
		
		void 	init();

		void 	pushConsigneAngle(int32_t );
		void 	pushConsigneDistance(int32_t );
		void 	changeIemeConsigne(int32_t , int32_t ,int16_t i);		
		void setNbConsignes(int16_t nbConsignesDonne);
		void 	changeIemeConsigneDistance (int32_t,int16_t i);
		void 	changeIemeConsigneAngle (int32_t,int16_t i);

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

		int32_t distanceTotale;
		int32_t angleTotal;

		int32_t	angleBkp;

		int32_t	distanceBkp;

		typedef struct {
			int32_t distance;
			int32_t angle;
		}Consigne;

		typedef struct {
			uint16_t nbConsignes;
			Consigne listeConsignes[NB_MAX_CONSIGNES] ;
		}TableauConsignes;

		uint16_t indiceConsigneActuelle;
		TableauConsignes tableauConsignes;

};

/*
 * Coordonnees du robot
 */ 
 
#define CONVERSION_ANGLE 1000
#define CONVERSION_DISTANCE 1000

extern int32_t x;
extern int32_t y;

extern Manager 			manager;

#endif
