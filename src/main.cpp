<<<<<<< HEAD
/*
Projet: rouler
Equipe: stationnement P-2
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
Robot: 2A
*/

/*
Inclure les librairies de functions que vous voulez utiliser
*/
#include <LibRobus.h>
#include "pid.h"
#include "ledModule.h"
#include "AttenteIR.h"
#include "suiveurDeLigne.h"
#include "couleur.h"
#include "mur.h"
#include "pince.h"

/*
Variables globales et defines
 -> defines...
 -> L'ensemble des fonctions y ont acces
*/

bool bumperArr;
int vertpin = 48;
int rougepin = 49;

void setup(){
  BoardInit();
  InitLedModule();
  Serial.begin(9600);
  
  //initialisation
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
  delay(100);
  beep(2);

  // initialisation des servomoteurs
  SERVO_Enable(0);
  SERVO_Enable(1);

  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}


void loop() {
  // UpdateLedModule();
//   if(demandeDemarrage == false)
//   {
//     while(lireManette(REMOTE_read()) != 1)
//     {
//       delay(10);
//     }
//     demandeDemarrage = true;
//   }
  arret();
  switch(GetZoneColor())
  {
    case CZ_RED: // (ROSE)
        // Code pour la zone rouge TOMBER LA QUILLE
        break;
    case CZ_YELLOW:
        contourner_mur(35);
        break;
    case CZ_ORANGE:
        trouver_verre();
        delay(10);
        break;
    case CZ_BLUE:
        if(demandeReveil == false)
        {
            while(lireManette(REMOTE_read()) != 2)
            {
                beep(1); // Prouver que qqch est recu mais que ce n'est pas la bonne demande
                delay(10);
            }
            demandeReveil = true;
        }
        break;
    case CZ_GREEN:
        avance(85);
        break;
    case CZ_OTHER:
        suivre_ligne();
        break;
    default:
        arret(); // Temporaire, à remplacer par le code approprié pour gérer les autres couleurs
        break;
  }
  delay(10);
}
=======
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
#include "quilleModule.h"




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
  BoardInit();
  InitQuilleModule();
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() 
{
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  ExecuteQuilleModule();
  
  delay(5000);// Delais pour décharger le CPU
}




/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

>>>>>>> origin/Quille
