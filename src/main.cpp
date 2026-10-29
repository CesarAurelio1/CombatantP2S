#include <Arduino.h>
#include <LibRobus.h>

// Codes infrarouges attendus
uint32_t codeDemarrage = 0x00FF8877;
uint32_t codeReveil = 0x00FF44BB;

// Demandes qui seront traitées par le programme principal
bool demandeDemarrage = false;
bool demandeReveil = false;


// Lire la manette et activer la demande correspondante
void lireManette()
{
    uint32_t message = REMOTE_read();

    // Aucune commande ou simple trame de répétition NEC
    if (message == 0 || message == 0xFFFFFFFF)
    {
        return;
    }

    // Afficher le code pour vérifier ce que le robot reçoit
    Serial.print("Code IR recu : 0x");
    Serial.println(message, HEX);

    if (message == codeDemarrage)
    {
        demandeDemarrage = true;
    }
    else if (message == codeReveil)
    {
        demandeReveil = true;
    }
    else
    {
        Serial.println("Message non reconnu");
    }
}


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
    // Vérifier à chaque passage si un message a été reçu
    lireManette();

    if (demandeDemarrage)
    {
        Serial.println("Demande de demarrage recue");

        // Le traitement du démarrage sera ajouté ici

        // Effacer la demande après son traitement
        demandeDemarrage = false;
    }

    if (demandeReveil)
    {
        Serial.println("Demande de reveil recue");

        // Le traitement du réveil sera ajouté ici

        demandeReveil = false;
    }
}