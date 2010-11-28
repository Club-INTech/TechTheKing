#include <avr/interrupt.h>
#include <avr/io.h>
#include "Wire.h"

#define ENCA (1 << PORTB2)
#define ENCB (1 << PORTB4)

#define MASQUE 0B0111100

#define ADRESSE 42

volatile int32_t encodeur = 0;

volatile char etatPins = 0;

uint8_t reponse[4];


void setup()
{
	// Initialisation de la communication I2C en slave
	Wire.begin(ADRESSE);
	Wire.onRequest(envoiReponse);
	
	// Initialisation de l'interruption
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT18) | (1 << PCINT20);
	
	// Initialisation de la communication série
	Serial.begin(9600);
}

void loop()
{
	delay(1000);
	Serial.print(encodeur);
	Serial.print("\n");
}

// Fonction d'interruption sur les codeurs
ISR(PCINT2_vect)
{
	unsigned char changementPins = (PIND & MASQUE) ^ etatPins;
	etatPins = PIND & MASQUE;
	
	if (changementPins & ENCA)
	{
		if (etatPins & ENCA)
		//front montant codeur voix A
		(etatPins & ENCB)?encodeur++:encodeur--;
		else
		//front descendant codeur voix A
		(etatPins & ENCB)?encodeur--:encodeur++;
	}
	
	if (changementPins & ENCB)
	{
		if (etatPins & ENCB)
		//front montant codeur voix B
		(etatPins & ENCA)?encodeur--:encodeur++;
		else
		//front descendant codeur voix B
		(etatPins & ENCA)?encodeur++:encodeur--;
	}
}

// Envoi d'une réponse à la carte asservissement
void envoiReponse()
{
	// Construction de la réponse
	reponse[0] = (uint8_t) (encodeur);
	reponse[1] = (uint8_t) (encodeur >> 8); 
	reponse[2] = (uint8_t) (encodeur >> 16);
	reponse[3] = (uint8_t) (encodeur >> 24);
	
	// Envoi de la réponse, en 4 octets 
	Wire.send(reponse, 4);
}
