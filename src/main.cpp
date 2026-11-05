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

// Zones deja vues (index = Color_Zone, sans CZ_OTHER). Chaque zone n'apparait
// qu'une fois dans le parcours : en revoir une veut dire que le tour est fait.
// Le depart peut etre n'importe ou, donc on ne se fie pas a une couleur precise.
bool zoneVue[CZ_OTHER] = {false};

// Tour complet : relacher ce que tient la pince puis arreter pour de bon.
void finDuTour()
{
  arret();
  Serial.println("Tour complet : pince relachee, arret"); // DEBUG
  CLAW_open();

  while (true)
  {
    delay(1000);
  }
}

void setup(){
  BoardInit();
  InitLedModule();
  InitQuilleModule();
  Serial.begin(9600);
  InitColorSensor(); // Sinon la 1re lecture arrive avant que le capteur soit pret (0 0 94)

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

  // Depart : attendre DEMARRAGE avant de commencer loop()
  attendreCommande(COMMANDE_DEMARRAGE);
}


void loop()
{
  arret();

  // Une seule lecture par tour : la DEL et le switch voient la meme couleur
  printColorValues = true; // DEBUG
  enum Color_Zone zone = GetZoneColor();
  printColorValues = false; // DEBUG
  Serial.print("loop: zone = "); Serial.println(zone); // DEBUG (0=R 1=J 2=O 3=B 4=V 5=autre)

  // Le vert ressemble au plancher : sa DEL s'allume seulement apres la
  // 2e verification (voir case CZ_GREEN)
  if (zone != CZ_GREEN)
  {
    UpdateLedModule(zone);
  }

  // Zone deja vue : tour complet (le vert est traite dans son case, apres sa
  // 2e verification). Arreter est definitif, donc on confirme par une 2e
  // lecture, robot immobile.
  if (zone != CZ_OTHER && zone != CZ_GREEN && zoneVue[zone])
  {
    delay(50);
    if (GetZoneColor() == zone)
    {
      finDuTour();
    }
    return; // Lecture douteuse : on relit au prochain tour de loop()
  }

  if (zone != CZ_OTHER && zone != CZ_GREEN)
  {
    zoneVue[zone] = true;
  }

  switch(zone)
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
      // Robot deja arrete par arret() : attendre REVEIL.
      // followLine ignore les couleurs pendant 4 s, ce qui sort le robot du bleu.
      attendreCommande(COMMANDE_REVEIL);
      Serial.println("bleu: REVEIL recu, followLine..."); // DEBUG
      followLine(0.17, GetZoneColor);
      Serial.println("bleu: followLine termine"); // DEBUG
      break;
    case CZ_GREEN:
      // 2e verification : avancer de 3 cm et relire, car le vert ressemble au plancher
      Serial.println("vert: avance(3)..."); // DEBUG
      avance(3);
      printColorValues = true; // DEBUG
      zone = GetZoneColor();
      Serial.print("vert: 2e lecture = "); Serial.println(zone); // DEBUG

      // DEBUG : 5 lectures de plus, 50 ms d'ecart, pour voir si la couleur
      // change une fois le robot stabilise (n'influence pas la decision)
      for (int i = 0; i < 5; i++)
      {
        delay(50);
        GetZoneColor();
      }
      printColorValues = false; // DEBUG

      if(zone == CZ_GREEN)
      {
        UpdateLedModule(CZ_GREEN);

        if (zoneVue[CZ_GREEN])
        {
          finDuTour(); // Vert deja vu : tour complet
        }
        zoneVue[CZ_GREEN] = true;

        Serial.println("vert: turn_degrees(10, 0)..."); // DEBUG
        turn_degrees(5, 0); // Se placer pour retrouver la ligne
        Serial.println("vert: avance(100)..."); // DEBUG
        avance(100);         // La ligne disparait sur environ 90 cm
        Serial.println("vert: turn_degrees(10, 1)..."); // DEBUG
        Serial.println("vert: fini"); // DEBUG
      }
      break;
      
    case CZ_OTHER:
      followLine(0.17, GetZoneColor);
      break;

    default:
      beep(1);
      arret(); // Temporaire, à remplacer par le code approprié pour gérer les autres couleurs
      break;
  }
  delay(10);
}