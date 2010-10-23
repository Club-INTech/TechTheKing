#include "MonClavier.h"

int8_t Clavier::lectureTouche( uint8_t *touche )
{
	byte dat = this->read();
	switch( dat )
	{
		// ceci est une reine (A==Q)
		case 'A' :
			*touche = 1;
			break;
			
		//ceci est un roi
		case 'K' :
			*touche = 0;
			break;
		default :
			return -1;
	}   
	return 1;
}
