#ifndef ATTENTEIR_MODULE_H
#define ATTENTEIR_MODULE_H

extern bool demandeDemarrage;
extern bool demandeReveil;

// Valeurs retournees par lireManette() (0 = rien ou code inconnu)
const int COMMANDE_DEMARRAGE = 1;
const int COMMANDE_REVEIL = 2;

int lireManette(uint32_t message);
void attendreCommande(int commandeAttendue);
void beep(int count);

#endif