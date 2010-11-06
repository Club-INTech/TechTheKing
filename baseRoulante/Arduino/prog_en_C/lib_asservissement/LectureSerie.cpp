#include "LectureSerie.h"

LectureSerie::LectureSerie() 
{
}

/*
 * Fonction à mettre dans le loop()
 */

/* les nom des fonctions du manager ne sont pas corrects
 ex: pour 'a' on avance et non on change l'angle */
void
LectureSerie::traitement() {
	unsigned char premierCaractere;
	while (Serial.available() == 0) {
		asm("nop");
	}
	premierCaractere = Serial.read();
	
	long int i;
	switch (premierCaractere) {
	case '?':
		Serial.println("0");
		break;
	case 'a':
		litEntierLong(&i);
		if (i >= 0)
			// changer la position
			manager.changeConsigneAngle(i);
		break;
	case 'b':
		litEntierLong(&i);
		if (i >= 0)
			// changer l'angle
			manager.changeConsigneDistance(i);
		break;
	case 'c':
		envoiPosition.active();
		break;
	case 'd':
		envoiPosition.desactive();
		break;
	case 'e':
		encodeurG = 0;
		encodeurD = 0;
		break;
	case 'f':
		litEntierLong(&i);
		if (i >= 0)
			manager.changeConsigneAngle(-i);
		break;
	case 'g':
		litEntierLong(&i);
		if (i >= 0)
			manager.changeConsigneDistance(-i);
		break;	
	case 'h':
		manager.switchAssDistance();
		break;
	case 'i':
		manager.switchAssAngle();
		break;
	case 'j':
		manager.reset();
		break;
	case 'k':
		litEntierLong(&i);
		if (i >= 0) {
			
		}
		break;
	case 'l':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assRotation.changeVmax(i);
		}
		break;
	case 'm':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assRotation.changeKp(i);
		}
		break;
	case 'n':
		manager.assRotation.stop();
		manager.assTranslation.stop();
		break;
	case 'o':
		cli();
		manager.assRotation.stopUrgence(encodeurG - encodeurD);
		manager.assTranslation.stopUrgence(encodeurG + encodeurD);
		sei();
		break;
	case 'p':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assRotation.changePWM(i);
		}
		break;
	case 'q':
		litEntierLong(&i);
		if (i >= 0) {
			
		}
		break;
	case 'r':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assTranslation.changeVmax(i);
		}
		break;
	case 's':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assTranslation.changeKp(i);
		}
		break;
	case 't':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assTranslation.changePWM(i);
		}
		break;
	case 'u':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assRotation.changeKd(i);
		}
		break;
	case 'v':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assTranslation.changeKd(i);
		}
		break;

	case 'w':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assTranslation.changeKi(i);
		}
		break;
		
	case 'x':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assTranslation.changeKi(i);
		}
		break;

	default:
		break;
	}
}

bool
LectureSerie::litEntierLong(long int *i)
{
	long int aux = 0;
	unsigned char j;
	long int k = 10000000;
	unsigned char c = 0;
	for (j = 0; j < 8; j++) {
		while (Serial.available()==0) { 
			asm("nop");
		}
		c = Serial.read();
		if (c < 48 || c > 57) {
			aux = -1;
			break;
		}
		aux += (c - 48) * k;
		k /= 10;
	} 
	*i = aux;
}

LectureSerie lectureSerie;
