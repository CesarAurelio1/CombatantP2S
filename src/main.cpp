/*
Projet: rouler
Equipe: stationnement P-w
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

uint8_t right = 0;
uint8_t left = 1;

int distance_case = 48; //cm
uint8_t orientation = 0; // 0 = nord, 1 = est, 2 = sud, 3 = ouest
uint8_t erreur_distance = 0;
int position = 1; // commence sur la case 1, on imagine une map de 0 à 29, donc 30 cases
uint16_t degree = 0;


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
  Serial.begin(9600);
  
  orientation = 0;
  //initialisation
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
  delay(100);
  beep(2);
}


void loop() {
  avance(50);
  delay(1000);
}