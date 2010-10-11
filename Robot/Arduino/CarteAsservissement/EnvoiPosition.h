/**
 * \file EnvoiPosition.h
 * \brief header de EnvoiPosition.cpp
 */

#ifndef EnvoiPosition_h
#define EnvoiPosition_H

#include <HardwareSerial.h>

#define TRIGGER_BLOCAGE	50

/**
 * \brief Se charge d'envoyer la position calculée au PC
 */
class EnvoiPosition {
	public:
		EnvoiPosition();
		
		void	boucle();
		void 	active();
		void	desactive();
	
	private:
		void 	intToHex(unsigned char *);
		bool	actif; //!< true si on envoie les positions
};

extern EnvoiPosition envoiPosition;

#endif
