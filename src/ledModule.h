// ledModule.h
#ifndef LED_MODULE_H
#define LED_MODULE_H

#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include "couleur.h"

/*enum Color_Zone {
  CZ_RED,
  CZ_YELLOW,
  CZ_ORANGE,
  CZ_BLUE,
  CZ_GREEN,
  CZ_OTHER
};*/

void InitLedModule(void);
// Allume la DEL de la zone recue (lue une seule fois dans loop())
void UpdateLedModule(enum Color_Zone zone);

#endif /* LED_MODULE_H */