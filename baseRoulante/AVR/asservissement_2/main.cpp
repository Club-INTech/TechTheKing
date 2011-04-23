/**
 * \file MainAsservissement.cppt
 * \brief Permet de controler l'asservissement
 *
 * exécute les ordres qui lui sont passées, voici la liste des ordres :
 * "?" pour demaner quelle carte est-tu (ie 0)
 * "a" avance en
 * "b" tourne jusqu'a
 * "e" réinitialise la position
 * "f" f recule en
 * "g" tourne jusqu'a negatif
 * "h" change AssAngle d'état
 * "i" change AssRotation d'état
 * "j" reset
 * "k" change l'accélération maximale en translation
 * "l" change le Vmax pour la translation
 * "m" change le Kp pour la translation
 * "n" stop tout mouvement
 * "o" s'asservit sur place
 * "p" change le PWM pour la transtation
 * "q" change l'accélération maximale en rotation
 * "r" change le Vmax pour la rotation
 * "s" change le Kp pour la rotation 
 * "t" change le PWM pour la rotation
 * "u" change le Kd pour la translation
 * "v" change le Kd pour la rotation
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib/Asservissement.h"
#include "util/delay.h"
#include "lib/LectureSerie.h"
#include "lib/Manager.h"
#include "lib/Util.h"
#include "lib/twi_master.h"
#include "lib/serial.h"

int32_t litEntierLong()
{
	int32_t aux = 0;
	unsigned char j=0;
	int32_t k = 10000000;
	unsigned char c;
	for (j = 0; j < 8; j++) {
		while (available()==0) { 
			asm("nop");
		}
		c = read();
		printlnLong(c);
		/*if (c < 48 || c > 57) {
			aux = -1;
			break;
		}*/
		aux += (c - 48) * k;
		k /= 10;
	}
	return aux;
}

int main( void ){
    // Série
    uart_init();
    // I2C
    TWI_init();
    //Manager
    manager.init();
    // Interruptions
    sei();

    send_reset();
    
    int32_t i=0;
    
    while(1)
    {
        unsigned char premierCaractere;
		while (available() == 0) {
			asm("nop");
		}
		premierCaractere = read();
		
		switch (premierCaractere) {
		case '?':
			printlnLong(0);
			break;
		case 'a':
			printlnLong(i);
			i=litEntierLong();
			printlnLong(i);
			if (i >= 10000000)
				//  avance
				manager.changeIemeConsigneAngle(i-10000000,1);
			else if(i>=0)
				// recule
				manager.changeIemeConsigneAngle(-i,1);
			break;
		case 'b':
			i=litEntierLong();
			if (i >= 10000000)
				//  tourne positivement de i
				manager.changeIemeConsigneDistance(i-10000000,1);
			else
			if (i >= 0)
				// tourne nÃ©gativement i
				manager.changeIemeConsigneDistance(-i,1);
			break;
		case 'e': // désactive interruptions
			manager.switchAssDistance();
			manager.switchAssAngle();
			break;
		case 'f': // A faire avant de charger une liste de points
			manager.setNbConsignes(00000000);
			break;
		case 'g': // push consigne etape 1
			i=litEntierLong();
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
			send_reset();
			break;
		case 'k':
			manager.test();
			break;
		case 'l':
			i=litEntierLong();
			if (i >= 0) {
				manager.assRotation.changeVmax(i);
			}
			break;
		case 'm':
			i=litEntierLong();
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
			send_reset();
			break;
		case 'p':
			i=litEntierLong();
			if (i >= 0) {
				manager.assRotation.changePWM(i);
			}
			break;
		case 'q': // push consigne (étape 2)
			i=litEntierLong();
			if (i >= 10000000)
			//  avance
				manager.pushConsigneAngle(i-10000000);
			else if(i>=0)
			// recule
				manager.pushConsigneAngle(-i);
			break;
		case 'r':
			i=litEntierLong();
			if (i >= 0) {
				manager.assTranslation.changeVmax(i);
			}
			break;
		case 's':
			i=litEntierLong();
			if (i >= 0) {
				manager.assTranslation.changeKp(i);
			}
			break;
		case 't':
			i=litEntierLong();
			if (i >= 0) {
				manager.assTranslation.changePWM(i);
			}
			break;
		case 'u':
			i=litEntierLong();
			if (i >= 0) {
				manager.assRotation.changeKd(i);
			}
			break;
		case 'v':
			i=litEntierLong();
			if (i >= 0) {
				manager.assTranslation.changeKd(i);
			}
			break;
		case 'w':
			i=litEntierLong();
			if (i >= 0) {
				manager.assTranslation.changeKi(i);
			}
			break;
		
		case 'x':
			i=litEntierLong();
			if (i >= 0) {
				manager.assRotation.changeKi(i);
			}
			break;
	
		case 'y':
			i=litEntierLong();
			if (i >= 0) {
				manager.assTranslation.changeVmax(i);
			}
	
		case 'z':
			i=litEntierLong();
			if (i >= 0) {
				manager.assRotation.changeVmax(i);
			}

		default:
			break;
		}
    }
}
