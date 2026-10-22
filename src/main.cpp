#include <Arduino.h>
#include <LibRobus.h>
#include <couleur.h>

void setup() {
  BoardInit();
  
  Serial.begin(9600);
}

void loop()
{
    couleur();
}
