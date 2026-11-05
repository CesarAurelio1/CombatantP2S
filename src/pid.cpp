#include "pid.h"
#include <LibRobus.h>
#include "couleur.h"

static float progression = 0;

static const unsigned long PERIODE_MS = 10;

static float previous_errorD = 0;
static float previous_errorG = 0;
static float integralD = 0;
static float integralG = 0;
static float Kp = 0.00002;
static float Ki = 0.00005;
static float Kd = 0.00005;
static float Ksync = 0.0005;

static float KpL = 0.25;
static float KiL = 0.0;
static float KdL = 0.0;


//static float tolerance = 10;

static float d_roue = 7.72; // cm
static float circ_roue = d_roue * PI; // cm
static float d_cercle = 18.1; // cm
static float circ_cercle = d_cercle * PI; // cm
static long nombre_pasD = 0;
static long nombre_pasG = 0;
static int pulses_per_revolution = 3200;

static float vitesse_min = 0.1;
static float vitesse_max = 0.3;
static float vitesse = 0;

static int suiveurGauche = A0;
static int suiveurCentre = A1;
static int suiveurDroite = A2;


float pid(float offset, int current, float target, float Kp, float Ki, float Kd, float &previous_error, float &integral) 
{
  float error = target - current;

  float correctif = offset * Ksync;

  float p = float(Kp) * float(error);
  integral += error * 0.02;
  float i = Ki * integral;
  float derivative = (error - previous_error) / 0.02;
  float d = Kd * derivative;
  previous_error = error;
  return p + i + d + correctif;
}

resultat pid_ligne(float offset, float Kp, float Ki, float Kd, float &previous_error, float &integral) 
{
  resultat r;
  float p = 0;
  float i = 0;
  float d = 0;

  p = Kp * offset;
  integral += offset * 0.02;
  i = Ki * integral;
  float derivative = (offset - previous_error) / 0.02;
  d = Kd * derivative;
  previous_error = offset;

  float pid = p + i + d;

  r.pidD = -1 * pid;
  r.pidG = pid;
  // float p = float(Kp) * float(offset);
  // integral += offset * 0.02;
  // float i = Ki * integral;
  // float derivative = (offset - previous_error) / 0.02;
  // float d = Kd * derivative;
  // previous_error = offset;
  // return p + i + d;
  return r;
}

float calculVitesse(float progression)
{
  if (progression < 0.4) {
    return vitesse_min + (vitesse_max - vitesse_min) * (progression / 0.4);
  }
  else if (progression > 0.7) {
    return vitesse_max - (vitesse_max - vitesse_min) * ((progression - 0.7) / 0.3);
  }
  else {
    return vitesse_max;
  }
}

void arret() 
{
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
}

void augmenter_Kp(float valeur)
{
  Kp += valeur;
}
void diminuer_Kp(float valeur)
{
  Kp -= valeur;
}
void augmenter_Ki(float valeur)
{
  Ki += valeur;
}
void diminuer_Ki(float valeur)
{
  Ki -= valeur;
}
void augmenter_Kd(float valeur)
{
  Kd += valeur;
}
void diminuer_Kd(float valeur)
{
  Kd -= valeur;
}
void augmenter_Ksync(float valeur)
{
  Ksync += valeur;
}
void diminuer_Ksync(float valeur)
{
  Ksync -= valeur;
}

void avance(float distance) {
  progression = 0;
  previous_errorD = 0;
  previous_errorG = 0;
  integralD = 0;
  integralG = 0;
  nombre_pasD = 0;
  nombre_pasG = 0;

  float offsetD = 0;
  float offsetG = 0;

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  unsigned long prochain = millis();
    
  float cible = (distance / circ_roue) * pulses_per_revolution;

  while (nombre_pasD < cible || nombre_pasG < cible) {

    if (nombre_pasD > cible)
    {
      MOTOR_SetSpeed(RIGHT, 0);
    }
    
    else if (nombre_pasG > cible)
    {
      MOTOR_SetSpeed(LEFT, 0);
    }

    offsetD = nombre_pasG - nombre_pasD;
    offsetG = nombre_pasD - nombre_pasG;
    progression = (float(abs(nombre_pasG))/cible + float(abs(nombre_pasD))/cible)/2;
    vitesse = calculVitesse(progression);
    float pidD = constrain(vitesse + pid(offsetD, nombre_pasD, cible, Kp, Ki, Kd, previous_errorD, integralD),-0.3, vitesse_max);
    float pidG = constrain(vitesse + pid(offsetG, nombre_pasG, cible, Kp, Ki, Kd, previous_errorG, integralG), -0.3, vitesse_max);

    MOTOR_SetSpeed(RIGHT, pidD);
    MOTOR_SetSpeed(LEFT, pidG);

    nombre_pasD += ENCODER_ReadReset(RIGHT);
    nombre_pasG += ENCODER_ReadReset(LEFT);

    prochain += PERIODE_MS;
    long attente = (long)(prochain - millis());
    if (attente > 0) delay(attente);
  }

  arret();
}

void recule(float distance) {
  progression = 0;
  previous_errorD = 0;
  previous_errorG = 0;
  integralD = 0;
  integralG = 0;
  nombre_pasD = 0;
  nombre_pasG = 0;

  float offsetD = 0;
  float offsetG = 0;

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  unsigned long prochain = millis();
    
  float cible = (distance / circ_roue) * pulses_per_revolution;

  while (nombre_pasD < cible || nombre_pasG < cible) {

    if (nombre_pasD > cible)
    {
      MOTOR_SetSpeed(RIGHT, 0);
    }
    
    else if (nombre_pasG > cible)
    {
      MOTOR_SetSpeed(LEFT, 0);
    }

    offsetD = nombre_pasG - nombre_pasD;
    offsetG = nombre_pasD - nombre_pasG;
    progression = (float(abs(nombre_pasG))/cible + float(abs(nombre_pasD))/cible)/2;
    vitesse = calculVitesse(progression);
    float pidD = constrain(vitesse + pid(offsetD, nombre_pasD, cible, Kp, Ki, Kd, previous_errorD, integralD), -0.3, vitesse_max-0.15);
    float pidG = constrain(vitesse + pid(offsetG, nombre_pasG, cible, Kp, Ki, Kd, previous_errorG, integralG), -0.3, vitesse_max-0.15);

    MOTOR_SetSpeed(RIGHT, -pidD);
    MOTOR_SetSpeed(LEFT, -pidG);

    nombre_pasD += labs(ENCODER_ReadReset(RIGHT));
    nombre_pasG += labs(ENCODER_ReadReset(LEFT));

    prochain += PERIODE_MS;
    long attente = (long)(prochain - millis());
    if (attente > 0) delay(attente);
  }

  arret();
}

static float derniere_erreur = 0;

// À mesurer : valeurs lues HORS ligne (blanc) et SUR la ligne, pour chaque capteur
static float minG = 0.10, maxG = 0.90;
static float minC = 0.10, maxC = 0.90;
static float minD = 0.12, maxD = 0.90;

static float norm(int brut, float mn, float mx)
{
  float v = (brut / 1024.0 - mn) / (mx - mn);
  return constrain(v, 0.0, 1.0);          // 0 = hors ligne, 1 = sur la ligne
}

float calcul_erreur() {
  float g = norm(analogRead(suiveurGauche), minG, maxG);
  float m = norm(analogRead(suiveurCentre), minC, maxC);
  float d = norm(analogRead(suiveurDroite), minD, maxD);

  float somme = g + m + d;

  // Si perte de ligne : conserver le dernier rayon de braquage sans saut
  if (somme < 0.15) {
    return derniere_erreur;
  }

  // Calcul d'erreur centré continu
  float err_brute = (d - g);

  // Application d'un lissage continu au lieu d'un saut discontinu
  const float deadband = 0.05;
  if (fabs(err_brute) < deadband) {
    err_brute = 0.0;
  } else if (err_brute > 0) {
    err_brute -= deadband;
  } else {
    err_brute += deadband;
  }

  derniere_erreur = err_brute;
  return derniere_erreur;
}

void suivre_ligne()
{
  float previous_error = 0;
  float integral = 0;
  float erreur = 0;

  unsigned long prochain = millis();
  unsigned long dernierAffichage = 0;

  while (GetZoneColor() == CZ_OTHER)
  {
    erreur = calcul_erreur();
    resultat correction;
    
    correction = pid_ligne(erreur, KpL, KiL, KdL, previous_error, integral);
    

    float vitesseCourante = vitesse_max - 0.1 - (vitesse_max - vitesse_min) * (fabs(erreur)/2);

    float pidD = constrain(vitesseCourante + correction.pidD, -0.1, vitesse_max);
    float pidG = constrain(vitesseCourante + correction.pidG, -0.1, vitesse_max);

    MOTOR_SetSpeed(LEFT, pidG);
    MOTOR_SetSpeed(RIGHT, pidD);

    if (millis() - dernierAffichage > 200) {
      dernierAffichage = millis();
      Serial.print("G: "); Serial.print(pidG);
      Serial.print(" | D: "); Serial.println(pidD);
    }

    // DANS la boucle : cadence fixe de 20 ms
    prochain += PERIODE_MS;
    long attente = (long)(prochain - millis());
    if (attente > 0) delay(attente);
  }

  arret();
}