// ledModule.h
#ifndef LED_MODULE_H
#define LED_MODULE_H

#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS

enum Color_Zone {
  CZ_RED,
  CZ_YELLOW,
  CZ_ORANGE,
  CZ_BLUE,
  CZ_GREEN,
  CZ_OTHER
};

void InitLedModule(void);
void UpdateLedModule(void);

#endif /* LED_MODULE_H */