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
#include "quilleModule.h"
#include "suiveurDeLigne.h"
#include "pid.h"

struct QuillleModuleData_t
{
    uint16_t irSensor1Value;
    uint8_t irSensor1Index;
    uint8_t irSensor1Pin;
    int16_t totalRotation;
    int16_t forwardMovement;
    bool quilleFound;
    bool quilleKnockOver;
};


static void ReadIRSensor(void);
static void ProcessIRSensorData(void);
static void Rotate(void);
static void MoveForward(void);
static void ReverseMovement(void);



static const uint8_t IR_SENSOR_1_PIN = A3;//A0 à A3 en fonction du branchement
static const uint8_t ROTATION_ANGLE = 20;
static const uint8_t MIN_DETECTION_VALUE = 400;
static const uint8_t FORWARD_MOVEMENT_VALUE_CM = 50;

static bool moduleExecutionCompleted = false;
static struct QuillleModuleData_t moduleData = {0,
                                                0,
                                                IR_SENSOR_1_PIN,
                                                0,
                                                0,
                                                false,
                                                false};


void InitQuilleModule(void)
{
    pinMode(moduleData.irSensor1Pin, INPUT);
}

void ExecuteQuilleModule(void)
{
    while (!moduleExecutionCompleted)
    {
        if (!moduleData.quilleFound)
        {
            ReadIRSensor();
            ProcessIRSensorData();

            if (!moduleData.quilleFound)
            {
                Rotate();

                if (moduleData.totalRotation >= 360)
                {
                    //permet de sortir de la boucle dans le cas ou la quille n'est pas détectée
                    moduleExecutionCompleted = true;
                }
            }
        }

        if (moduleData.quilleFound && !moduleData.quilleKnockOver)
        {
            ReverseMovement();
            
            moduleData.quilleKnockOver = true;
        }

        if (moduleData.quilleKnockOver && !moduleExecutionCompleted)
        {
            MoveForward();

            moduleExecutionCompleted = true;
        }

        delay(10);
    }
}

static void ReadIRSensor(void)
{
    moduleData.irSensor1Value = ROBUS_ReadIR(moduleData.irSensor1Index);

    //Correction pour le capteur de distance, parfois la valeur lu à distance saute 
    //jusqu'à une valeur de 900 à 1000 qu<on doit filtrer.
    if (moduleData.irSensor1Value > 600)
    {
        moduleData.irSensor1Value = 100;
    }
}

static void ProcessIRSensorData(void)
{
    if (moduleData.irSensor1Value > MIN_DETECTION_VALUE )//TODO : calibrer pour touver la bonne valeur de distance
    {
        moduleData.quilleFound = true;
    }
}

static void Rotate(void)
{
    turn_degrees(ROTATION_ANGLE, 0);

    moduleData.totalRotation += ROTATION_ANGLE;
}

static void MoveForward(void)
{
    avance(FORWARD_MOVEMENT_VALUE_CM);

    moduleData.forwardMovement += FORWARD_MOVEMENT_VALUE_CM;
}

static void ReverseMovement(void)
{
    recule(moduleData.forwardMovement);

    turn_degrees(moduleData.totalRotation, 1);
}