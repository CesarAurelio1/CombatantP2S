#include <Arduino.h>
#include <LibRobus.h>
#include "AttenteIR.h"

// Lire la manette et activer la demande correspondante
void lireManette(uint32_t message)
{
    // Afficher le code pour vérifier ce que le robot reçoit
    Serial.print("Code IR recu : 0x");
    Serial.println(message, HEX);

    if (message == DEMARRAGE)
    {
        beep(2);
        demandeDemarrage = true; // Remettre à false après traitement dans le programme principal
        Serial.println("Demande de demarrage recue");
    }
    else if (message == REVEIL)
    {
        beep(3);
        demandeReveil = true; // Remettre à false après traitement dans le programme principal
        Serial.println("Demande de reveil recue");
    }
    else
    {
        Serial.println("Code IR non reconnu");
    }
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