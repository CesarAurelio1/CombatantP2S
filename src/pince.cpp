#include "pince.h"
#include <LibRobus.h>
#include <Servo.h>

Servo servo1; 
uint8_t const P_OUVERT = 0;
uint8_t const P_FERME = 90;
#define SERVO_PIN = A4;
int distance_capteur = 0;

void fermer_pince()
{
  servo1.write(P_FERME);
  delay(200);
}

void ouvrir_pince()
{
  servo1.write(P_OUVERT);
  delay(200);
}

void trouver_verre()
{
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  while (distance_capteur > 250)
  {
    MOTOR_SetSpeed(RIGHT, 0.2);
    MOTOR_SetSpeed(LEFT, -0.2);
  }
  if (distance_capteur < 250)
  {
    avance(distance_capteur-100);
    fermer_pince();
    tourner(180);
    avance(distance_capteur-100);
  }
}
