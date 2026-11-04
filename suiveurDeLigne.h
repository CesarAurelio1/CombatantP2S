// SUIVEURDELIGNE.h
#ifndef SUIVEURDELIGNE_H
#define SUIVEURDELIGNE_H

#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS

void followLine(float vitesse_de_croisiere, String (*condition)());

void CLAW_init();

void CLAW_open();

void CLAW_close();

#endif // SUIVEURDELIGNE_H
