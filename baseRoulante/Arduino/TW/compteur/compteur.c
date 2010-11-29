#include "compteur.h"

// Initialisation de l'interruption
void compteurSetup()
{
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); 
}

// Fonction d'interruption sur les codeurs
ISR(PCINT2_vect)
{

  unsigned char changementPins = (PIND & MASQUE) ^ etatPins;
  etatPins = PIND & MASQUE;
  
  if (changementPins & ENCGA)
  {
    if (etatPins & ENCGA)
      //front montant codeur 1 voix A
      (etatPins & ENCGB)?encodeurG++:encodeurG--;
    else
      //front descendant codeur 1 voix A
      (etatPins & ENCGB)?encodeurG--:encodeurG++;
  }

  if (changementPins & ENCGB)
  {
    if (etatPins & ENCGB)
      //front montant codeur 1 voix B
      (etatPins & ENCGA)?encodeurG--:encodeurG++;
    else
      //front descendant codeur 1 voix B
      (etatPins & ENCGA)?encodeurG++:encodeurG--;
  }

  if (changementPins & ENCDA)
  {
    if (etatPins & ENCDA) 
      //front montant codeur 2 voix A
      (etatPins & ENCDB)?encodeurD++:encodeurD--;
    else
      //front descendant codeur 2 voix A
      (etatPins & ENCDB)?encodeurD--:encodeurD++;
   }
 
  if (changementPins & ENCDB)
  {
    if (etatPins & ENCDB)
      //front montant codeur 2 voix B
      (etatPins & ENCDA)?encodeurD--:encodeurD++;
    else
      //front descendant codeur 2 voix B
      (etatPins & ENCDA)?encodeurD++:encodeurD--;
  }
}

// Fonction de remise à zéro (Utile. Ou pas...)
void compteurReset()
{
  encodeurG = 0;
  encodeurD = 0;
}
