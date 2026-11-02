#include "pince.h"
#include "pid.h"
#include "suiveurDeLigne.h"
#include <LibRobus.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
uint8_t const SERVO_PIN1 = A4;
uint8_t const SERVO_PIN2 = A6;
uint8_t const CAPTEUR_PIN = A5;
signed int distance_capteur = 0;
uint8_t const ANGLE_OUVERT =  67;
uint8_t const ANGLE_FERME =  69;
uint8_t distance_avancer = 0;
//oublier pas d'attach servo1 à la pin 1 pi meme affaire pour 2

void fermer_pince()
{
  servo1.write(ANGLE_FERME);
  delay(200);
}

void ouvrir_pince()
{
  servo1.write(ANGLE_OUVERT);
  delay(200);
}

void lever_pince()
{
  for (int angle = 2; angle <= 90; angle += 2)
  {
    servo2.write(angle);
    delay(20);
  }

  delay(200);
}

void trouver_verre()
{
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  distance_capteur = analogRead(CAPTEUR_PIN);

  while (distance_capteur < 231)
  {
    distance_capteur = analogRead(CAPTEUR_PIN);
    MOTOR_SetSpeed(RIGHT, 0.2);
    MOTOR_SetSpeed(LEFT, -0.2);
  }
  if (distance_capteur > 231)
  {
    distance_capteur = analogRead(CAPTEUR_PIN);
    distance_avancer = ((distance_capteur*100)/532);
    avance(distance_avancer-70);
    fermer_pince();

    turn_degrees(180, true); // A CHANGER
    avance(distance_avancer-70);
  }
}
