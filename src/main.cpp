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


void beep(int count){
  for(int i=0;i<count;i++){
    AX_BuzzerON();
    delay(100);
    AX_BuzzerOFF();
    delay(100);  
  }
  delay(400);
}

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

  Serial.println("Robot pret a recevoir la manette");

  Serial.begin(BAUD_RATE_SERIAL0);
  Serial.println("Debug communication initialized");
  InitRemoteControlModule();
}


void loop() {
  UpdateLedModule();
  // Lire la manette et activer la demande correspondante
  lireManette(REMOTE_read());
  UpdateRemoteControlModule();
  suivre_ligne();
}
