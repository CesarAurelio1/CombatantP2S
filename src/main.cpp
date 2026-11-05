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
  CLAW_init();
  CLAW_open();

  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);

  analogReference(INTERNAL2V56);

  // Depart : attendre REVEIL puis DEMARRAGE avant de commencer loop()
  attendreReveilPuisDemarrage();
}


void loop()
{
  UpdateLedModule();

  arret();
  switch(GetZoneColor())
  {
    case CZ_RED: // (ROSE)
      recule(10);
      trouver_quille();
      followLine(0.17, GetZoneColor);

      break;
    case CZ_YELLOW:
      contourner_mur(40);
      followLine(0.17, GetZoneColor);

      break;
    case CZ_ORANGE:
    recule(10);
      CLAW_open();
      trouver_verre();
      delay(10);
      followLine(0.17, GetZoneColor);

        break;
    case CZ_BLUE:
      // Robot deja arrete par arret() : attendre REVEIL puis DEMARRAGE.
      // followLine ignore les couleurs pendant 4 s, ce qui sort le robot du bleu.
      attendreReveilPuisDemarrage();
      followLine(0.17, GetZoneColor);
      break;
    case CZ_GREEN:
      avance(3);
      if(GetZoneColor() == CZ_GREEN)
      {
        turn_degrees(10, 0);
        avance(100);
        turn_degrees(10, 1);
      }
      break;
    case CZ_OTHER:
      followLine(0.17, GetZoneColor);
      break;
    default:
      arret(); // Temporaire, à remplacer par le code approprié pour gérer les autres couleurs
      break;
  }
  delay(10);
}