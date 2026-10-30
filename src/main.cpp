/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <Arduino.h>
#include <LibRobus.h>

#include "remoteControlModule.h"

/* ****************************************************************************
struct et enum
**************************************************************************** */

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */

/* ****************************************************************************
Déclaration de fonctions ici
**************************************************************************** */

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales
void setup() {
  // put your setup code here, to run once:
  // Initialize debug communication on Serial0
  Serial.begin(BAUD_RATE_SERIAL0);
  Serial.println("Debug communication initialized");
  InitRemoteControlModule();
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"
void loop() {
  // put your main code here, to run repeatedly:
  UpdateRemoteControlModule();

  delay(10);
}


/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

