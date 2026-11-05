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

void SleepRobot(void);

bool bumperArr;
int vertpin = 48;
int rougepin = 49;
bool sleep = true;
bool start = false;

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

  SleepRobot();
}


void loop() 
{
    // Zone bleue : le robot dort jusqu'au bouton REVEIL
  if (sleep == true)
  {
    lireManette(REMOTE_read());

    if (demandeReveil == true)
    {
      sleep = false;
      demandeReveil = false;

            // Sortir de la zone bleue (followLine ignore les couleurs pendant
            // 4 s), sinon le robot la revoit et se rendort aussitot.
      followLine(0.17, GetZoneColor);
    }

    delay(10);

    return;
  }
  // Demarrage initial : attend le bouton DEMARRAGE (une seule fois)
  if (start == false)
  {
    // Etalonnage : affiche les valeurs du capteur de couleur pendant que
    // le robot attend, pour pouvoir le poser sur chaque couleur sans qu'il bouge.
    static unsigned long derniereLecture = 0;
    if (millis() - derniereLecture > 250)
    {
        derniereLecture = millis();
        GetZoneColor();
    }

    lireManette(REMOTE_read());

    if (demandeDemarrage == true)
    {
        start = true;
        demandeDemarrage = false;
    }

    delay(10);
        
    return;
  }

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
      while(lireManette(REMOTE_read()) != 1)
      {
        delay(10);
      }

      while(lireManette(REMOTE_read()) != 2)
      {
        delay(10);
      }
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

void SleepRobot(void)
{
    sleep = true;
    start = false;
    arret();
}