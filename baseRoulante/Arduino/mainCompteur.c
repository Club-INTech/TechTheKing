#include "compteur.h"
#include "comTW.h"

void setup()
{
  comTWSetup();
  compteurSetup();
  Serial.begin(9600);
}

void loop()
{
}
