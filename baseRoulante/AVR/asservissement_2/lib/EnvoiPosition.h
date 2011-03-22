#ifndef EnvoiPosition_H
#define EnvoiPosition_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include "serial.h"
#include "Manager.h"
#include "Asservissement.h"
#include <stdint.h>

class EnvoiPosition {
	public:
		EnvoiPosition();
		
		void	boucle();
		void 	active();
		void	desactive();
	
	private:
		void 	intToHex(unsigned char *);
		
		bool	actif;
};

extern EnvoiPosition envoiPosition;

#endif
