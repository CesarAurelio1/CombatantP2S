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
#include "remoteControlModule.h"
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
bool vert = false;
bool rouge = false;
int etat = 0; 
int etatPast = 0;

bool partie = 0;

void setup(){
  BoardInit();
  InitLedModule();
  Serial.begin(9600);
  
  //initialisation
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
  delay(100);
  beep(2);

   MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}


void loop() {
  UpdateLedModule();
  if(partie == false)
  {
    while(lireManette(REMOTE_read()) != 1)
    {
      delay(10);
    }
    partie = true;
  }
  

  suivre_ligne();
}
