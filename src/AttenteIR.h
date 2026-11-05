#ifndef ATTENTEIR_MODULE_H
#define ATTENTEIR_MODULE_H

extern bool demandeDemarrage;
extern bool demandeReveil;

int lireManette(uint32_t message);
void attendreReveilPuisDemarrage();
void beep(int count);

#endif