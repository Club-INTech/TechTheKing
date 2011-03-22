#include "LectureSerie.h"

LectureSerie::LectureSerie() 
{
}

/*
 * Fonction Ã  mettre dans le loop()
 */

/* les nom des fonctions du manager ne sont pas corrects
 ex: pour 'a' on avance et non on change l'angle */
void
LectureSerie::traitement() {
	unsigned char premierCaractere;
	while (available() == 0) {
		asm("nop");
	}
	premierCaractere = read();
	
	int32_t i;
	switch (premierCaractere) {
	case '?':
		printChar('0');
		break;
	case 'a':
		litEntierLong(&i);
		if (i >= 10000000)
			//  avance
			manager.changeIemeConsigneAngle(i-10000000,1);
		else if(i>=0)
			// recule
			manager.changeIemeConsigneAngle(-i,1);
		break;
	case 'b':
		litEntierLong(&i);
		if (i >= 10000000)
			//  tourne positivement de i
			manager.changeIemeConsigneDistance(i-10000000,1);
		else if (i >= 0)
			// tourne nÃ©gativement i
			manager.changeIemeConsigneDistance(-i,1);
		break;
	case 'c':
		envoiPosition.active();
		break;
	case 'd':
		envoiPosition.desactive();
		break;
	case 'e': // désactive interruptions
		manager.switchAssDistance();
		manager.switchAssAngle();
		break;
	case 'f': // A faire avant de charger une liste de points
		manager.setNbConsignes(00000000);
		break;
	case 'g': // push consigne etape 1
		litEntierLong(&i);
		if (i >= 10000000)
		//  avance
			manager.pushConsigneDistance(i-10000000);
		else if(i>=0)
		// recule
			manager.pushConsigneDistance(-i);
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
		manager.test();
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
		manager.reset();
		break;
	case 'p':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assRotation.changePWM(i);
		}
		break;
	case 'q': // push consigne (étape 2)
		litEntierLong(&i);
		if (i >= 10000000)
		//  avance
			manager.pushConsigneAngle(i-10000000);
		else if(i>=0)
		// recule
			manager.pushConsigneAngle(-i);
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
			manager.assRotation.changeKi(i);
		}
		break;
	
	case 'y':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assTranslation.changeVmax(i);
		}
	
	case 'z':
		litEntierLong(&i);
		if (i >= 0) {
			manager.assRotation.changeVmax(i);
		}

	default:
		break;
	}
}

bool LectureSerie::litEntierLong(int32_t *i)
{
	int32_t aux = 0;
	unsigned char j;
	int32_t k = 10000000;
	unsigned char c = 0;
	for (j = 0; j < 8; j++) {
		while (available()==0) { 
			asm("nop");
		}
		c = read();
		if (c < 48 || c > 57) {
			aux = -1;
			break;
		}
		aux += (c - 48) * k;
		k /= 10;
	} 
	*i = aux;
	return true;
}

LectureSerie lectureSerie;
