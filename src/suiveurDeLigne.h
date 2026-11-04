// SUIVEURDELIGNE.h
#ifndef SUIVEURDELIGNE_H
#define SUIVEURDELIGNE_H

#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include "couleur.h"

void followLine(float vitesse_de_croisiere, enum Color_Zone (*condition)());
void turn_degrees(int, bool);
void CLAW_init();
void CLAW_open();
void CLAW_close();

#endif // SUIVEURDELIGNE_H
