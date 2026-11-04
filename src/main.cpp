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
#include "quilleModule.h"

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
  InitQuilleModule();
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

  analogReference(INTERNAL2V56);
}


void loop() 
{
    UpdateLedModule();
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
        ExecuteQuilleModule();
        break;
    case CZ_YELLOW:
        contourner_mur(35);
        break;
    case CZ_ORANGE:
        // trouver_verre();
        // delay(10);
        // break;
    case CZ_BLUE:
        if(demandeReveil == false)
        {
            while(lireManette(REMOTE_read()) != 2)
            {
                delay(10);
            }
            demandeReveil = true;
        }
        break;
    case CZ_GREEN:
        avance(85);
        break;
    case CZ_OTHER:
        followLine(0.17, GetZoneColor);
        break;
    default:
        arret(); // Temporaire, à remplacer par le code approprié pour gérer les autres couleurs
        break;
  }

}
