// SUIVEURDELIGNE.h
#ifndef SUIVEURDELIGNE_H
#define SUIVEURDELIGNE_H

#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS

void followLine(float vitesse_de_croisiere, int (*condition)());

extern float Kp_suiveur;
extern float Ki_suiveur;
extern float Kd_suiveur;

#endif // SUIVEURDELIGNE_H