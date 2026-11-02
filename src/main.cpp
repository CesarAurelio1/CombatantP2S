#include <Arduino.h>
#include <LibRobus.h>
#include <couleur.h>   // Gives access to enum Color_Zone and GetZoneColor()

void setup() {
  BoardInit();
  Serial.begin(9600);   // Needed for Serial.println in couleur.cpp
  InitColorSensor();    // Start the color sensor once
}

void loop()
{
  // Read the sensor and get the zone as an enum value
  enum Color_Zone color = GetZoneColor();

  switch (color)
  {
    case CZ_RED:    /* action for red */    break;
    case CZ_YELLOW: /* action for yellow */ break;
    case CZ_ORANGE: /* action for orange */ break;
    case CZ_BLUE:   /* action for blue */   break;
    case CZ_GREEN:  /* action for green */  break;
    case CZ_OTHER:  /* no color recognised */ break;
    default:        /* should never happen */ break;
  }

  delay(10);   // Small delay to ease the CPU load
}