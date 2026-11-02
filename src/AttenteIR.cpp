#include <Arduino.h>
#include <LibRobus.h>
#include "AttenteIR.h"

// Codes infrarouges attendus
const uint32_t DEMARRAGE = 0x807F8877;  // Code pour la demande de démarrage initiale du robot (0x11)
const uint32_t REVEIL = 0x807F44BB;     // Code pour la demande de réveil du robot lors de la détection du bleu (0x22)

// Demandes qui seront traitées par le programme principal
bool demandeDemarrage = false;
bool demandeReveil = false;

// Lire la manette et activer la demande correspondante
int lireManette(uint32_t message)
{
    if (message == DEMARRAGE)
    {
        beep(2);
        demandeDemarrage = true; // Remettre à false après traitement dans le programme principal
        return 1;
    }
    else if (message == REVEIL)
    {
        beep(3);
        demandeReveil = true; // Remettre à false après traitement dans le programme principal
        return 2;
    }
    if (message != 0) {
        // Afficher le code pour vérifier ce que le robot reçoit
        return 0;
    }
    return 0;
}

// Fonction pour émettre un signal sonore avec le buzzer
void beep(int count){
  for(int i=0;i<count;i++){
    AX_BuzzerON();
    delay(100);
    AX_BuzzerOFF();
    delay(100);  
  }
  delay(400);
}