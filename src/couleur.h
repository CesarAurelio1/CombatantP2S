#ifndef COULEUR_H
#define COULEUR_H

#include <Arduino.h>

/* Zones the color sensor can recognise.
   This enum is shared with the main file, so it lives in the header.
   CZ_OTHER is returned when no reference color is close enough. */
enum Color_Zone {
  CZ_RED,
  CZ_YELLOW,
  CZ_ORANGE,
  CZ_BLUE,
  CZ_GREEN,
  CZ_OTHER
};

// When true, every reading prints "R G B -> zone" on the serial port.
extern bool printColorValues;

// Call once in setup(): starts the TCS34725 sensor.
void InitColorSensor();

// Reads the sensor and returns the matching zone (or CZ_OTHER).
enum Color_Zone GetZoneColor();

// Compares raw {R, G, B} values to the references and returns the closest zone.
enum Color_Zone detect_color(const uint16_t color_values[]);

#endif