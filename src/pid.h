#ifndef PID_MODULE_H
#define PID_MODULE_H

struct resultat
{
  float pidD;
  float pidG;
};

float PID(float, int, float, float, float, float, float &, float &);
resultat PID_ligne(float, float, float, float, float &, float &);
float calculVitesse(float);
void avance(float);
void arret(void);
void augmenter_Kp(float);
void diminuer_Kp(float);
void augmenter_Ki(float);
void diminuer_Ki(float);
void augmenter_Kd(float);
void diminuer_Kd(float);
void augmenter_Ksync(float);
void diminuer_Ksync(float);
void followLine(void);
float calcul_erreur(void);

#endif