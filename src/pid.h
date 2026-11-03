#ifndef PID_MODULE_H
#define PID_MODULE_H

struct resultat
{
  float pidD;
  float pidG;
};

float pid(float, int, float, float, float, float, float &, float &);
resultat pid_ligne(float, float, float, float, float &, float &);
float calculVitesse(float);
void avance(float);
void recule(float);
void arret(void);
void augmenter_Kp(float);
void diminuer_Kp(float);
void augmenter_Ki(float);
void diminuer_Ki(float);
void augmenter_Kd(float);
void diminuer_Kd(float);
void augmenter_Ksync(float);
void diminuer_Ksync(float);
void suivre_ligne(void);
float calcul_erreur(void);

#endif