#include "EnvoiPosition.h"

EnvoiPosition::EnvoiPosition() 
{
	TCCR0A |= (1 << CS02) | (0 << CS01) | (1 << CS00);
	OCR0A = 128;
	
	TCNT0 = 0;
}

/*
 * Fonction à exécuter dans un timer
 */
void 
EnvoiPosition::boucle()
{
	/*
	 * Envoi de encodeurG, encodeurD si actif = 1
	 * Pour blocageDetecte, le passage de assRotation et assTranslation en public est moche
	 */
}

/*
 * Active le timer
 */
void 
EnvoiPosition::active()
{
	actif = true;
	TIMSK0 |=  (1 << OCIE0A);
}

/*
 * Désactive le timer
 */
void 
EnvoiPosition::desactive()
{
	actif = false;
	TIMSK0 &= ~(1 << OCIE0A);
}

void 
EnvoiPosition::intToHex(unsigned char *data)
{
	unsigned char c;
	unsigned char i; 
	
	for (i = 3; i >= 0; i--)
	{
		c = (data[i] & 0xF0) >> 4;
		
		if (c <= 9)
		{
			printChar(c + '0');
		}
		else
		{
			printChar(c - 10 + 'A');
		}
			
		c = data[i] & 0xF;
		
		if (c <= 9)
		{
			printChar(c + '0');
		}
		else
		{
			printChar(c - 10 + 'A');
		}
	}
}

/*
 * Portion à modifier
 * Utiliser : EnvoiPosition::boucle()
 */
 
// Division par 4 du temps de cette interruption @ 57600 bauds (pareil à 9600..)

char stator = 3;

int32_t bufferG;
int32_t bufferD;

ISR(TIMER0_COMPA_vect)
{
	if (stator == 0) {
		stator = 3;
		bufferG = x;
		bufferD = y;		
		sei();
		printLong(bufferG);
		printChar(' ');
		printLong(bufferD);
		printChar(' ');
		
		if (manager.assTranslation.blocageTemp == TRIGGER_BLOCAGE) {
			if (manager.assRotation.erreur > 0) 
				printChar('1');
			else
				printChar('2');
		}
		else if (manager.assTranslation.blocageTemp == -TRIGGER_BLOCAGE) {
			if (manager.assRotation.erreur > 0) 
			{
				printString("-2");
			}
			else
			{
				printString("-1");
			}	
		} 
		else {
			printChar('0');
		}
		
		printChar(' ');
		
		if (manager.assRotation.blocageTemp == TRIGGER_BLOCAGE) {
			if (manager.assTranslation.erreur > 0) 
				printChar('1');
			else
				printChar('2');
		}
		else if (manager.assRotation.blocageTemp == -TRIGGER_BLOCAGE) {
			if (manager.assTranslation.erreur > 0) 
			{
				printString("-2");
			}
			else
			{
				printString("-1");
			}
		} 
		else {
			printChar('0');
		}

		println();
	}
	else {
		stator--;
	}	
}

EnvoiPosition envoiPosition;

