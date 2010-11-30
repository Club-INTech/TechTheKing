#include "accesCompteur.h"

#define ADRESSE 42

// Ces fonctions servent à récupérer les informations de la carte compteuse
// Compteur setup doit être appelée par la fonction setup de la carte

void compteurSetup()
{
  Wire.begin();
  Wire.onReceive(carteCompteur);
}


long int distance()
{
  int32_t reponseCompteur
  
  // L'envoi de O au compteur signifie que l'on demande l'angle
  Wire.beginTransmission(ADRESSE);
  Wire.send(0);
  Wire.endTransmission();
  
  // On demande l'envoi de la distance, codée sur 4 octets
  Wire.requestFrom(ADRESSE, 4);
  
  while (Wire.available() = 4)
  {
    // Reception de reponseCompteur, octet par octet
    reponseCompteur |= ((int32_t) Wire.receive());
    reponseCompteur |= ((int32_t) Wire.receive() << 8);
    reponseCompteur |= ((int32_t) Wire.receive() << 16);
    reponseCompteur |= ((int32_t) Wire.receive() << 24);
  }
  
  return reponseCompteur;
}


long int angle()
{
  int32_t reponseCompteur
  
  // L'envoi de 1 au compteur signifie que l'on demande l'angle
  Wire.beginTransmission(ADRESSE);
  Wire.send(1);
  Wire.endTransmission();
  
  // On demande l'envoi de l'angle, codé sur 4 octets
  Wire.requestFrom(ADRESSE, 4);
  
  while (Wire.available() = 4)
  {
    // Reception de reponseCompteur, octet par octet
    reponseCompteur |= ((int32_t) Wire.receive());
    reponseCompteur |= ((int32_t) Wire.receive() << 8);
    reponseCompteur |= ((int32_t) Wire.receive() << 16);
    reponseCompteur |= ((int32_t) Wire.receive() << 24);
  }
  
  return reponseCompteur;
}
