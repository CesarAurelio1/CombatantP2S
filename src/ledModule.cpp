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
#include "ledModule.h"
#include "couleur.h"
#include "TimerThree.h"

static enum Color_Zone GetLEDColor();
static void DisplayColor(uint8_t pin);
static void ClearColorDisplay(void);
static void Timer3ISR(void);
static void timer_start(void);
static void timer_stop(void);


static const uint8_t RED_LED_PIN = 42;
static const uint8_t YELLOW_LED_PIN = 41;
static const uint8_t ORANGE_LED_PIN = 40;
static const uint8_t BLUE_LED_PIN = 43;
static const uint8_t GREEN_LED_PIN = 44;

static const uint16_t LED_DISPLAY_TIME_MS = 10000;

static enum Color_Zone currentColor = CZ_OTHER;
static enum Color_Zone lastColor = CZ_OTHER;
static unsigned long timestamp = 0;

static bool colorIsDisplayed = false;
static uint8_t elapsedSeconds = 0;


void InitLedModule(void)
{
    pinMode(RED_LED_PIN, OUTPUT);
    digitalWrite(RED_LED_PIN, HIGH);

    pinMode(YELLOW_LED_PIN, OUTPUT);
    digitalWrite(YELLOW_LED_PIN, HIGH);

    pinMode(ORANGE_LED_PIN, OUTPUT);
    digitalWrite(ORANGE_LED_PIN, HIGH);

    pinMode(BLUE_LED_PIN, OUTPUT);
    digitalWrite(BLUE_LED_PIN, HIGH);

    pinMode(GREEN_LED_PIN, OUTPUT);
    digitalWrite(GREEN_LED_PIN, HIGH);

    colorIsDisplayed = false;
}

void UpdateLedModule(void)
{
    currentColor = GetLEDColor();

    if (currentColor != lastColor)
    {
        switch (currentColor)
        {
            case CZ_RED:
            {
                DisplayColor(RED_LED_PIN);
                break;
            }
            case CZ_YELLOW:
            {
                DisplayColor(YELLOW_LED_PIN);
                break;
            }
            case CZ_ORANGE:
            {
                DisplayColor(ORANGE_LED_PIN);
                break;
            }
            case CZ_BLUE:
            {
                DisplayColor(BLUE_LED_PIN);
                break;
            }
            case CZ_GREEN:
            {
                DisplayColor(GREEN_LED_PIN);
                break;
            }
            case CZ_OTHER:
            {
                //couleur inconnue ou hors treshold
                break;
            }
            default :
            {
                //erreur, ne devrais jamais passé ici, logger l'erreur
                break;
            }
        }

        lastColor = currentColor;
    }
}

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
static enum Color_Zone GetLEDColor()
{
    return GetZoneColor();
}

static void DisplayColor(uint8_t pin)
{
    ClearColorDisplay();

    timestamp = millis();

    digitalWrite(pin, LOW);

    colorIsDisplayed = true;

    timer_start();
}

static void ClearColorDisplay(void)
{
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    digitalWrite(ORANGE_LED_PIN, HIGH);
    digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);

    colorIsDisplayed = false;
}

static void Timer3ISR(void)
{
    elapsedSeconds++;

    if (elapsedSeconds >= 10)
    {
        ClearColorDisplay();

        timer_stop();
    }
}

static void timer_start(void) 
{
    elapsedSeconds = 0;
    Timer3.initialize(1000000); // période en µs : 1 s
    Timer3.attachInterrupt(Timer3ISR);
}

static void timer_stop(void) 
{
    Timer3.stop();
    Timer3.detachInterrupt();
}