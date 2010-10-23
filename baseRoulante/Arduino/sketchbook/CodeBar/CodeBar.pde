#include <MonClavier.h>

Clavier clavier;
uint8_t touche;

void setup()
{

	/*
	* Activation du clavier
	*/
	clavier.begin();
	Serial.begin(9600);
}

void loop()
{
	if( clavier.available() )
	{
		// si touche vaut 1  c'est un roi sinon c'est une reine
		if( clavier.lectureTouche( &touche ) == 1 )
		{ 
			Serial.println((short)touche);
		}
	}  
}
