#include "pince.h"
#include "pid.h"
#include "suiveurDeLigne.h"
#include <LibRobus.h>

uint8_t const SERVO_PIN1 = A4;
uint8_t const SERVO_PIN2 = A6;
uint8_t const CAPTEUR_PIN = A3;
signed int distance_capteur = 0;
uint8_t const ANGLE_OUVERT =  67;
uint8_t const ANGLE_FERME =  69;
uint8_t distance_avancer = 0;
//oublier pas d'attach servo1 à la pin 1 pi meme affaire pour 2

void fermer_pince()
{
  SERVO_SetAngle(0, ANGLE_FERME);
  delay(200);
}

void ouvrir_pince()
{
  SERVO_SetAngle(0, ANGLE_OUVERT);
  delay(200);
}

void lever_pince()
{
  for (int angle = 2; angle <= 90; angle += 2)
  {
    SERVO_SetAngle(1, angle);
    delay(20);
  }

  delay(200);
}

void trouver_verre()
{
  // 1. Arrêt initial de sécurité
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);

  // 2. Lecture initiale du capteur de distance sur A3
  distance_capteur = analogRead(CAPTEUR_PIN);
  Serial.print("Valeur capteur initiale : ");
  Serial.println(distance_capteur);

  long total_pasD = 0;

  // Lancement de la rotation initiale
  MOTOR_SetSpeed(RIGHT, 0.1);
  MOTOR_SetSpeed(LEFT, -0.1);

  // 3. Pivoter TANT QUE le capteur ne détecte pas l'obstacle (< 400)
  while (distance_capteur < 400)
  {
    delay(10);
    distance_capteur = analogRead(CAPTEUR_PIN);
    
    // Accumulation des pas de la roue droite pendant la recherche
    total_pasD += labs(ENCODER_ReadReset(RIGHT));
  }

  // 4. Arrêter la rotation continue dès que l'obstacle est détecté
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(100);

  // 5. Ajustement de 10 degrés
  turn_degrees(10, 1); 
  delay(100);

  // 6. Mouvements d'approche / recul vers la quille
  recule(30);
  CLAW_close();
  avance(30);

  // 7. Rotation de RETOUR (Sens inverse de la recherche)
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);

  long pas_retour = 0;

  // Si on a accumulé du déplacement, on fait tourner en sens inverse
  if (total_pasD > 0)
  {
    MOTOR_SetSpeed(RIGHT, -0.1);
    MOTOR_SetSpeed(LEFT, 0.1);

    while (pas_retour < total_pasD)
    {
      delay(10);
      // Cumul positif des pas effectués lors du retour
      pas_retour += labs(ENCODER_ReadReset(RIGHT));
    }

    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
  }

  // Annulation des 10 degrés d'ajustement du pas 5 (sens 0 pour annuler le sens 1)
  turn_degrees(10, 0);
}

void trouver_quille()
{
  // 1. Arrêt initial de sécurité
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);

  // 2. Lecture initiale du capteur de distance sur A3
  distance_capteur = analogRead(CAPTEUR_PIN);
  Serial.print("Valeur capteur initiale : ");
  Serial.println(distance_capteur);

  long total_pasD = 0;

  // Lancement de la rotation initiale
  MOTOR_SetSpeed(RIGHT, 0.1);
  MOTOR_SetSpeed(LEFT, -0.1);

  // 3. Pivoter TANT QUE le capteur ne détecte pas l'obstacle (< 400)
  while (distance_capteur < 400)
  {
    delay(10);
    distance_capteur = analogRead(CAPTEUR_PIN);
    
    // Accumulation des pas de la roue droite pendant la recherche
    total_pasD += labs(ENCODER_ReadReset(RIGHT));
  }

  // 4. Arrêter la rotation continue dès que l'obstacle est détecté
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(100);

  // 5. Ajustement de 10 degrés
  turn_degrees(10, 1); 
  delay(100);

  // 6. Mouvements d'approche / recul vers la quille
  recule(25);
  avance(25);

  // 7. Rotation de RETOUR (Sens inverse de la recherche)
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);

  long pas_retour = 0;

  // Si on a accumulé du déplacement, on fait tourner en sens inverse
  if (total_pasD > 0)
  {
    MOTOR_SetSpeed(RIGHT, -0.1);
    MOTOR_SetSpeed(LEFT, 0.1);

    while (pas_retour < total_pasD)
    {
      delay(10);
      // Cumul positif des pas effectués lors du retour
      pas_retour += labs(ENCODER_ReadReset(RIGHT));
    }

    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
  }

  // Annulation des 10 degrés d'ajustement du pas 5 (sens 0 pour annuler le sens 1)
  turn_degrees(10, 0);
}