#include <Arduino.h>
#include <LibRobus.h>
#include "AttenteIR.h"

void setup()
{
    BoardInit();

    // Les moteurs restent arrêtés pendant ce premier test
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    Serial.println("Robot pret a recevoir la manette");
}


void loop()
{
    // Lire la manette et activer la demande correspondante
    lireManette(REMOTE_read());
    delay(100); // Petite pause pour éviter de saturer le port série
}