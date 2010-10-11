/**
 * \file MainAsservissement.cpp
 * \brief Permet de controler l'asservissement
 *
 * exécute les ordres qui lui sont passées, voici la liste des ordres :\n\n
 * "?" pour demaner quelle carte est-tu (ie 0)\n
 * "a" avance en\n
 * "b" tourne jusqu'a\n
 * "c" active l'envoie de la position\n
 * "d" désactive l'envoie de la position\n
 * "e" réinitialise la position\n
 * "f" f recule en\n
 * "g" tourne jusqu'a negatif\n
 * "h" change AssAngle d'état\n
 * "i" change AssRotation d'état\n
 * "j" reset\n
 * "k" change l'accélération maximale en translation\n
 * "l" change le Vmax pour la translation\n
 * "m" change le Kp pour la translation\n
 * "n" stop tout mouvement\n
 * "o" s'asservit sur place\n
 * "p" change le PWM pour la transtation\n
 * "q" change l'accélération maximale en rotation\n
 * "r" change le Vmax pour la rotation\n
 * "s" change le Kp pour la rotation\n
 * "t" change le PWM pour la rotation\n
 * "u" change le Kd pour la translation\n
 * "v" change le Kd pour la rotation\n
 */

#include <Asservissement.h>
#include <EnvoiPosition.h>
#include <LectureSerie.h>
#include <Manager.h>

/**
 * \brief initialise la connection série à 57600 bauds
 */
void setup()
{
	Serial.begin(57600);
        manager.init();
}

/**
 * \brief réalise le traitement suivant en permanence
 */
void loop()
{
	lectureSerie.traitement();
}
