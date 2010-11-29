#include "accesCompteur.h"

void setup()
{
  compteurSetup();
}

void loop()
{
  delay(1000);
  Serial.print((int)angle());
  Serial.write("\n");
  Serial.print((int)distance());
  Serial.write("\n");
}
