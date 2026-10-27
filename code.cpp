/*
  Code par Gael S.

  2026-10-15
*/

#include <Arduino.h>
#include <LibRobus.h>

static int suiveurGauche = A0;
static int suiveurCentre = A1;
static int suiveurDroite = A2;

static float Kp_suiveur = 0.5;
static float Ki_suiveur = 0;
static float Kd_suiveur = 0;

int couleur()
{
  if (millis() < 3000)
  {
    return 5;
  }
  else
  {
    return 5;
  }
}

struct PID_return
{
  float vitesse1;
  float vitesse2;
};

float calcul_erreur()
{
  float erreur = 0;

  float gauche = analogRead(suiveurGauche) / 1024.0;
  float milieu = analogRead(suiveurCentre) / 1024.0;
  float droite = analogRead(suiveurDroite) / 1024.0;

  erreur = (gauche - droite) * (1 - milieu);

  return erreur;
}

float limite_variation_vitesse(float vitesse, float &vitesse_precedante)
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

PID_return PID(float vitesse_cible, float erreur, long &integrale, float &erreur_precedante, unsigned long &timestamp, float Kp, float Ki, float Kd)
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

void followLine(float vitesse_de_croisiere)
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

  while (couleur() == 5)
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

void setup()
{
  BoardInit();
  delay(100);

  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
}

void loop()
{
  followLine(0.35);
  delay(100);
}
