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
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include "ledModule.h"


/* ****************************************************************************
struct et enum
**************************************************************************** */

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces


/* ****************************************************************************
Déclaration de fonctions ici
**************************************************************************** */
//Les fonctions doivent être déclarées avant d'être appelées en c, c++,
//contrairement à un script Arduino qui le fait automatiquement en background.



/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales
void setup() 
{
  // put your setup code here, to run once:
  BoardInit();
  InitLedModule();
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"
void loop() 
{
  // put your main code here, to run repeatedly:

  UpdateLedModule();

  delay(10);
}

// put function definitions here:
