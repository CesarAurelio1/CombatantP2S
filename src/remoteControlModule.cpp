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

#include "remoteControlModule.h"

// --- Configuration (AVANT l'include) ---
#define DISABLE_CODE_FOR_RECEIVER               // Désactive les fonctionnalitées de réception
#define SEND_PWM_BY_TIMER                       // Utilise un pwm généré à l'aide d'un timer hardware au lieu de logiciel
#define IR_USE_AVR_TIMER3                       // Utilisation du timer 3 pour le pwm de la carier wave 38 kHz, ne pas utiliser le timer 2 qui est par défaut
/*
Timer	            Broche d’envoi
1	                    11
2 (défaut)	            9
3	                    5
4	                    6
5	                    46
*/
//Broche PWM qui pilote le gate / la base du transistor, NE PAS UTILISER LA PIN 9 (timer 2) car on utilise le buzzer (tone()) qui utilise ce timer
//#define IR_SEND_PIN 5                         // Pin de contrôle de la led infrarouge, dans notre cas, la base du transistor, actif à 5V(HIGH), la configuration est relié à IR_USE_AVR_TIMER3 en fonction du microcontrôleur
// #define USE_ACTIVE_LOW_OUTPUT_FOR_SEND_PIN   // À décommenter seulement si la LED IR s'active avec un niveau BAS
#define FEEDBACK_LED_IS_ACTIVE_LOW              // À décommenter seulement si la LED FEDBACK s'active avec un niveau BAS
// #define NO_LED_FEEDBACK_CODE                 // Pour désactiver complètement le feedback

#include <IRremote.hpp>

/* ****************************************************************************
struct et enum
**************************************************************************** */
struct Button_t
{
    bool debounce;
    unsigned long timestamp;
    uint8_t debounceDelay;
    Button_Callback_t callback;
    uint8_t currentState;
    uint8_t lastState;
};

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
static const uint8_t IR_ROBOT_ADDRESS = 0x00;
static const uint8_t IR_COMMAND_START = 0x11;
static const uint8_t IR_COMMAND_EXIT_WAIT_MODE = 0x22;
static const uint8_t IR_REPEAT = 0x00;

static const uint8_t IR_LED_ON = HIGH;
static const uint8_t IR_LED_OFF = LOW;
static const uint8_t COMM_LED_ON = LOW;
static const uint8_t COMM_LED_OFF = HIGH;


static const uint8_t COMMUNICATION_FEEDBACK_LED_PIN = 2;
static const uint8_t BTN_ROBOT_EXIT_WAIT_MODE_PIN = 3;
static const uint8_t BTN_ROBOT_START_PIN = 4;

static const uint8_t BUTTON_DEBOUNCE_TIME_MS = 50;
static const uint8_t BUTTON_PRESS_STATE = HIGH;
static const uint8_t BUTTON_RELEASE_STATE = LOW;


static struct Button_t btnRobotExitWaitMode;
static struct Button_t btnRobotStart;

/* ****************************************************************************
Déclaration de fonctions ici
**************************************************************************** */
static void OnBtnRobotExitWaitModeStateChange(void);
static void OnBtnRobotStartStateChange(void);
static void SendExitWaitModeSignal(void);
static void SendStartSignal(void);




void InitRemoteControlModule(void)
{
    IrSender.begin(COMMUNICATION_FEEDBACK_LED_PIN);

    pinMode(BTN_ROBOT_EXIT_WAIT_MODE_PIN, INPUT);
    pinMode(BTN_ROBOT_START_PIN, INPUT);


    btnRobotExitWaitMode.callback = OnBtnRobotExitWaitModeStateChange;
    btnRobotExitWaitMode.debounce = false;
    btnRobotExitWaitMode.debounceDelay = BUTTON_DEBOUNCE_TIME_MS;
    btnRobotExitWaitMode.currentState = digitalRead(BTN_ROBOT_EXIT_WAIT_MODE_PIN);
    btnRobotExitWaitMode.lastState = btnRobotExitWaitMode.currentState;
    btnRobotExitWaitMode.timestamp = millis();

    btnRobotStart.callback = OnBtnRobotStartStateChange;
    btnRobotStart.debounce = false;
    btnRobotStart.debounceDelay = BUTTON_DEBOUNCE_TIME_MS;
    btnRobotStart.currentState = digitalRead(BTN_ROBOT_START_PIN);
    btnRobotStart.lastState = btnRobotStart.currentState;
    btnRobotStart.timestamp = millis();
}

void UpdateRemoteControlModule(void)
{
    unsigned long currentMillis = millis();

    btnRobotExitWaitMode.currentState = digitalRead(BTN_ROBOT_EXIT_WAIT_MODE_PIN);
    btnRobotStart.currentState = digitalRead(BTN_ROBOT_START_PIN);

    if (btnRobotExitWaitMode.debounce == false)
    {
        if (btnRobotExitWaitMode.currentState != btnRobotExitWaitMode.lastState)
        {
            btnRobotExitWaitMode.debounce = true;
            btnRobotExitWaitMode.timestamp = currentMillis;
        }
    }
    else
    {
        if (currentMillis - btnRobotExitWaitMode.timestamp >= btnRobotExitWaitMode.debounceDelay)
        {
            btnRobotExitWaitMode.debounce = false;
            btnRobotExitWaitMode.lastState = btnRobotExitWaitMode.currentState;

            if (btnRobotExitWaitMode.callback != NULL)
            {
                btnRobotExitWaitMode.callback();
            }
        }
    }


    if (btnRobotStart.debounce == false)
    {
        if (btnRobotStart.currentState != btnRobotStart.lastState)
        {
            btnRobotStart.debounce = true;
            btnRobotStart.timestamp = currentMillis;
        }
    }
    else
    {
        if (currentMillis - btnRobotStart.timestamp >= btnRobotStart.debounceDelay)
        {
            btnRobotStart.debounce = false;
            btnRobotStart.lastState = btnRobotStart.currentState;

            if (btnRobotStart.callback != NULL)
            {
                btnRobotStart.callback();
            }
        }
    }
}

static void OnBtnRobotExitWaitModeStateChange(void)
{
    Serial.println("OnBtnRobotExitWaitModeStateChange");

    if (btnRobotExitWaitMode.currentState == BUTTON_PRESS_STATE)
    {
        Serial.println("Button exit wait mode is down");

        SendExitWaitModeSignal();
    }
    else
    {
        Serial.println("Button exit wait mode is up");
    }
    
}

static void OnBtnRobotStartStateChange(void)
{
    Serial.println("OnBtnRobotStartStateChange");

    if (btnRobotStart.currentState == BUTTON_PRESS_STATE)
    {
        Serial.println("Button start is down");

        SendStartSignal();
    }
    else
    {
        Serial.println("Button start is up");
    }
}

static void SendExitWaitModeSignal(void)
{
    IrSender.sendNEC(IR_ROBOT_ADDRESS, IR_COMMAND_EXIT_WAIT_MODE, IR_REPEAT);
}

static void SendStartSignal(void)
{
    IrSender.sendNEC(IR_ROBOT_ADDRESS, IR_COMMAND_START, IR_REPEAT);
}