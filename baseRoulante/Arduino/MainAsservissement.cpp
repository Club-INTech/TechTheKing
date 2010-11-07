/**
 * \file MainAsservissement.cpp
 * \brief Permet de controler l'asservissement
 *
 * exécute les ordres qui lui sont passées, voici la liste des ordres :
 * "?" pour demaner quelle carte est-tu (ie 0)
 * "a" avance en
 * "b" tourne jusqu'a
 * "c" active l'envoie de la position
 * "d" désactive l'envoie de la position
 * "e" réinitialise la position
 * "f" f recule en
 * "g" tourne jusqu'a negatif
 * "h" change AssTranslation d'état
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

#include <Asservissement.h>
#include <EnvoiPosition.h>
#include <LectureSerie.h>
#include <Manager.h>

/**
 * \brief initialise la connection série à 57600 bauds
 */
void setup() {
	Serial.begin(57600);
        manager.init();
}



void loop() {

	lectureSerie.traitement();

}
