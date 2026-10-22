#ifndef PID_MODULE_H
#define PID_MODULE_H


float PID(float, int, float, float, float, float, float &, float &);
float calculVitesse(float);
void avance(float);
void arret(void);

#endif