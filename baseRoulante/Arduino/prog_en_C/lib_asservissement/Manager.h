#ifndef Manager_h
#define Manager_h

#include <avr/interrupt.h>
#include <avr/io.h>
#include "../lib_avr/HardwareSerial.h"

#include "Asservissement.h"

// Puissance maximal de chaque moteur (1023 MAX)
#define PWM_MAX	1023

/*
 * Réglage des masques des codeurs
 * On utilise PORTB2 à 5
 */
#define ENCGA PORTD2
#define ENCGB PORTD4
#define ENCDA PORTD3
#define ENCDB PORTD5
#define ENC_PORT PORTD
#define ENC_CONF DDRD


#define MASQUE 0b0111100

/*
 * Réglage des pins des PWM
 */

// Roue Gauche
#define PIN_DIR_G PORTB3
#define PIN_PWM_G PORTB1

// Roue Droite
#define PIN_DIR_D PORTB4
#define PIN_PWM_D PORTB2

#define PORT_PWM_CONF DDRB
#define PORT_PWM PORTB

class Manager {
	public:
		Manager();
		
		void 	init();
		
		void 	changeConsigne (long int, long int);
		
		void 	changeConsigneDistance (long int);
		void 	changeConsigneAngle (long int);

		void 	assPolaire();
		
		void 	switchAssDistance();
		void 	switchAssAngle();
		
		void	reset();

		Asservissement 	assRotation;
		Asservissement 	assTranslation;
		
		// Activation de l'asservissement
		bool		activationAssDistance;
		bool		activationAssAngle;
};

extern volatile long int 	encodeurG;
extern volatile long int 	encodeurD;

extern Manager 			manager;

#endif
