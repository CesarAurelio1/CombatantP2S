/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */




/*
Code par Gael S.

2026-10-15
*/
#include "suiveurDeLigne.h"

#include <Arduino.h>
#include <LibRobus.h>

// Valeurs pour le suiveur de ligne
static int suiveurGauche = A0;
static int suiveurCentre = A1;
static int suiveurDroite = A2;

float Kp_suiveur = 0.5;
float Ki_suiveur = 0;
float Kd_suiveur = 0;

// Valeur pour tourner
static const uint8_t WHEEL_DIAMETER = 76;
static const uint8_t WHEEL_TRACK = 190;
static const uint16_t SQUARE_SIZE = 500;
static const uint16_t STEPS_PER_REVOLUTION = 3200;
static const uint16_t MAX_SPEED_PPS = 10666;
static const float FORWARD_SPEED = 0.5;
static const float TURN_SPEED = 0.3;

static const uint8_t STEPS_ACCURACY_THRESHOLD = 3;
static const float TURN_RATIO = 0.3205; // Constante valant un facteur magique * 0.25

const bool CLOCKWISE_TURN = true;
const bool COUNTERCLOCKWISE_TURN = false;

static float Kp_offset = 0.0002;

static float Kp_turn = 0.00065;
static float Ki_turn = 0.00000003;
static float Kd_turn = 0.00000002;


struct PID_return
{
  float vitesse1;
  float vitesse2;
};

static float calcul_erreur()
{
  float erreur = 0;

  float gauche = analogRead(suiveurGauche) / 1024.0;
  float milieu = analogRead(suiveurCentre) / 1024.0;
  float droite = analogRead(suiveurDroite) / 1024.0;

  erreur = (gauche - droite) * (1 - milieu);

  return erreur;
}

static float limite_variation_vitesse(float vitesse, float &vitesse_precedante)
{
  float variation = vitesse - vitesse_precedante;
  if (variation > 0.01)
  {
    vitesse = vitesse_precedante + 0.01;
  }
  else if (variation < -0.01)
  {
    vitesse = vitesse_precedante - 0.01;
  }
  vitesse_precedante = vitesse;
  return vitesse;
}

static PID_return PID(float vitesse_cible, float erreur, long &integrale, float &erreur_precedante, unsigned long &timestamp, float Kp, float Ki, float Kd)
{
  PID_return vitesses_sortie;

  float vitesseG;
  float vitesseD;

  float variation_ligne;

  float derivee;

  integrale += erreur / (millis() - timestamp + 1);

  derivee = (erreur - erreur_precedante) / (millis() - timestamp + 1);
  erreur_precedante = erreur;

  variation_ligne = erreur * Kp + integrale * Ki + derivee * Kd;

  vitesseG = vitesse_cible - variation_ligne;
  vitesseD = vitesse_cible + variation_ligne;

  timestamp = millis();
  vitesses_sortie.vitesse1 = vitesseG;
  vitesses_sortie.vitesse2 = vitesseD;
  return vitesses_sortie;
}

void followLine(float vitesse_de_croisiere, int (*condition)())
{
  /*
  Suit la ligne a vitesse constante

  Si la ligne devie vers la droite, la roue gauche ralentis et la droite accelere
  Si la ligne devie vers la gauche, la roue droite ralentis et la gauche accelere

  Oui   Map  la ligne comme une valeur entre -1 et 1
        -1 si ligne sous capteur gauche
        0 si ligne sous capteur centre
        1 si ligne sous capteur droite

  Oui   Variation max par iteration : +-0,01 (variable?)


  Non   Vise un output moyen de 0,3 (variable) aux MOTOR-set-speed
  Oui   --> vise 0 pour arreter

  Oui   Error pour PID : suiveur de ligne
  */

  unsigned long timestamp = millis();
  float erreur;
  float vitesse_M1;
  float vitesse_M2;
  float vitesse_precedant_M1 = 0;
  float vitesse_precedant_M2 = 0;
  long integrale = 0;
  float ancienne_erreur = 0;
  float start_time = timestamp;
  
  while (condition() == 5 || timestamp - start_time < 2000)
  {
    erreur = calcul_erreur();
    PID_return vitesses = PID(vitesse_de_croisiere, erreur, integrale, ancienne_erreur, timestamp, Kp_suiveur, Ki_suiveur, Kd_suiveur);
    vitesse_M1 = vitesses.vitesse1;
    vitesse_M2 = vitesses.vitesse2;
    vitesse_M1 = limite_variation_vitesse(vitesse_M1, vitesse_precedant_M1);
    vitesse_M2 = limite_variation_vitesse(vitesse_M2, vitesse_precedant_M2);
    MOTOR_SetSpeed(LEFT, vitesse_M1);
    MOTOR_SetSpeed(RIGHT, vitesse_M2);
    delay(1);
  }
  while (vitesse_M1 + vitesse_M2 > 0.05)
  {
    erreur = calcul_erreur();
    PID_return vitesses = PID(0, erreur, integrale, ancienne_erreur, timestamp, Kp_suiveur, Ki_suiveur, Kd_suiveur);
    vitesse_M1 = vitesses.vitesse1;
    vitesse_M2 = vitesses.vitesse2;
    vitesse_M1 = limite_variation_vitesse(vitesse_M1, vitesse_precedant_M1);
    vitesse_M2 = limite_variation_vitesse(vitesse_M2, vitesse_precedant_M2);
    MOTOR_SetSpeed(LEFT, vitesse_M1);
    MOTOR_SetSpeed(RIGHT, vitesse_M2);
    delay(1);
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

static float PID(float target, float current_value, long timestamp, float &previous_value, long &integral, float &previous_error, float Kp, float Ki, float Kd, float other_value, float speed_value)
{
  float error = target - current_value;
  float offset = current_value - other_value;
  float PID_return;
  float derivative;
  float proportional_out;
  float integral_out;
  float derivative_out;

  if (target > 0)
  {
    if (error > 600)
    {
      return speed_value - offset * Kp_offset;
    }
  }
  else
  {
    if (abs(error) > 600)
    {
      return -speed_value + offset * Kp_offset;
    }
  }

  integral = (integral / error) + (error * timestamp);
  derivative = (error - previous_error) / timestamp;
  previous_error = error;

  proportional_out = error * Kp;
  integral_out = integral * Ki;
  derivative_out = derivative * Kd;

  previous_value = current_value;

  PID_return = proportional_out + integral_out + derivative_out;
  return PID_return;
}


void turn_degrees(float degrees, bool direction)
{
  // steps_per_rotation (90 degree) = STEPS_PER_REVOLUTION * TURN_RATIO * (WHEEL_TRACK / WHEEL_DIAMETER);
  // steps_per_rotation = 3200*(0.3205/0.25)*(190/76)*(degrees voulu / 360);
  int steps_per_rotation = 10256 * (degrees / 360);
  float M1_speed = 0;
  float M2_speed = 0;
  float M1_previous_value;
  float M2_previous_value;
  unsigned long timestamp;
  long M1_integral = 0;
  float M1_previous_error = 0;
  long M2_integral = 0;
  float M2_previous_error = 0;
  int M1_target;
  int M2_target;

  if (direction)
  {
    M1_target = steps_per_rotation;
    M2_target = -steps_per_rotation;
    for (float i = 0; i <= 30; i++)
    {
      MOTOR_SetSpeed(LEFT, i / 60);
      MOTOR_SetSpeed(RIGHT, -i / 60);
      delay(20);
    }
  }
  else
  {
    M1_target = -(steps_per_rotation);
    M2_target = (steps_per_rotation);
    for (float i = 0; i <= 30; i++)
    {
      MOTOR_SetSpeed(LEFT, -i / 60);
      MOTOR_SetSpeed(RIGHT, i / 60);
      delay(20);
    }
  }
  timestamp = millis();
  M1_previous_value = ENCODER_Read(LEFT);
  M2_previous_value = ENCODER_Read(RIGHT);
  while ((abs(abs(M1_target) - abs(ENCODER_Read(LEFT))) > STEPS_ACCURACY_THRESHOLD) && (abs(abs(M2_target) - abs(ENCODER_Read(RIGHT))) > STEPS_ACCURACY_THRESHOLD))
  {
    M1_speed = PID(M1_target, ENCODER_Read(LEFT), 1000 * (millis() - timestamp), M1_previous_value, M1_integral, M1_previous_error, Kp_turn, Ki_turn, Kd_turn, -ENCODER_Read(RIGHT), TURN_SPEED);
    M2_speed = PID(M2_target, ENCODER_Read(RIGHT), 1000 * (millis() - timestamp), M2_previous_value, M2_integral, M2_previous_error, Kp_turn, Ki_turn, Kd_turn, -ENCODER_Read(LEFT), TURN_SPEED);
    MOTOR_SetSpeed(LEFT, M1_speed);
    MOTOR_SetSpeed(RIGHT, M2_speed);
    timestamp = millis();
    delay(2);
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
}




