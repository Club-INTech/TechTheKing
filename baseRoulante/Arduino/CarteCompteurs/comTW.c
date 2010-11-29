#include "Wire.h"
#include "comTW.h"

#define ADRESSE 42

// Initialisation de la communication I2C en slave
void comTWSetup()
{
  Wire.begin(42);
  Wire.onReceive(choixReponse);
  Wire.onRequest(envoiReponse);
}

// Choix du type de réponse par la carte asservissement
void choixReponse()
{
  // 0 pour la distance, 1 pour l'angle
  typeReponse = Wire.receive();
}

// Envoi d'une réponse à la carte asservissement
void envoiReponse()
{
  int32_t reponse = 0;
  
  if (typeReponse = DISTANCE)
    // Envoi de la distance
    reponse = encodeurG + encodeurD;
  
  if (typeReponse = ANGLE)
    // Envoi de l'angle
    reponse = encodeurG - encodeurD;
  
  // Envoi de la réponse, en 4 octets 
  Wire.send(&reponse, 4);
}
