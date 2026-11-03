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
    uint16_t irSensor2Value;
    uint8_t irSensor1Index;
    uint8_t irSensor2Index;
    uint8_t irSensor1Pin;
    uint8_t irSensor2Pin;
    int16_t totalRotation;
    int16_t forwardMovement;
    bool quilleFound;
    bool quilleKnockOver;
};


static void ReadIRSensor(void);
static void ProcessIRSensorData(void);
static void Rotate(void);



static const uint8_t IR_SENSOR_1_PIN = A0;//A0 à A3 en fonction du branchement
static const uint8_t IR_SENSOR_2_PIN = A1;//A0 à A3 en fonction du branchement
static const uint8_t ROTATION_ANGLE = 20;
static const uint8_t MIN_DETECTION_VALUE = 80;
static const uint8_t FORWARD_MOVEMENT_VALUE_CM = 50;

static bool moduleExecutionCompleted = false;
static struct QuillleModuleData_t moduleData = {.irSensor1Value = 0, 
                                                .irSensor2Value = 0, 
                                                .irSensor1Index = 0, 
                                                .irSensor2Index = 1, 
                                                .irSensor1Pin = IR_SENSOR_1_PIN, 
                                                .irSensor2Pin = IR_SENSOR_2_PIN,
                                                .quilleFound = false,
                                                .quilleKnockOver = false};


void InitQuilleModule(void)
{
    pinMode(moduleData.irSensor1Pin, INPUT);
    pinMode(moduleData.irSensor2Pin, INPUT);
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
            }
        }

        if (moduleData.quilleFound && !moduleData.quilleKnockOver)
        {
            MoveForward();
            
            moduleData.quilleKnockOver = true;
        }

        if (moduleData.quilleKnockOver && !moduleExecutionCompleted)
        {
            ReverseMovement();

            moduleExecutionCompleted = true;
        }

        delay(10);
    }
}

static void ReadIRSensor(void)
{
    moduleData.irSensor1Value = ROBUS_ReadIR(moduleData.irSensor1Index);
    moduleData.irSensor2Value = ROBUS_ReadIR(moduleData.irSensor2Index);
}

static void ProcessIRSensorData(void)
{
    if (moduleData.irSensor1Value > MIN_DETECTION_VALUE || moduleData.irSensor2Value > MIN_DETECTION_VALUE)//TODO : calibrer pour touver la bonne valeur de distance
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