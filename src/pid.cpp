#include "pid.h"
#include <LibRobus.h>

static float progression = 0;

static const unsigned long PERIODE_MS = 20;

static float previous_errorD = 0;
static float previous_errorG = 0;
static float integralD = 0;
static float integralG = 0;
static float Kp = 0.00002;
static float Ki = 0.00005;
static float Kd = 0.00005;
static float Ksync = 0.0005;

//static float tolerance = 10;

static float d_roue = 7.72; // cm
static float circ_roue = d_roue * PI; // cm
static float d_cercle = 18.1; // cm
static float circ_cercle = d_cercle * PI; // cm
static long nombre_pasD = 0;
static long nombre_pasG = 0;
static int pulses_per_revolution = 3200;

static float vitesse_min = 0.00;
static float vitesse_max = 0.6;
static float vitesse = 0;

float PID(float offset, int current, float target, float Kp, float Ki, float Kd, float &previous_error, float &integral) 
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
    float pidD = vitesse + PID(offsetD, nombre_pasD, cible, Kp, Ki, Kd, previous_errorD, integralD);
    float pidG = vitesse + PID(offsetG, nombre_pasG, cible, Kp, Ki, Kd, previous_errorG, integralG);

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