#include <Arduino.h>
#include "couleur.h"
#include <Adafruit_TCS34725.h>

/* ****************************************************************************
Sensor object
**************************************************************************** */
static Adafruit_TCS34725 colorSensor(
    TCS34725_INTEGRATIONTIME_2_4MS,
    TCS34725_GAIN_16X
);

/* ****************************************************************************
Reference color table
**************************************************************************** */
struct ColorReference {
  enum Color_Zone zone;      
  uint16_t rgb[3];           
};

// Vos 5 références calibrées pour 2.4ms
static const ColorReference REFERENCES[] = {
  { CZ_RED,    {205, 154, 130} },
  { CZ_YELLOW, {153, 171, 91} },
  { CZ_ORANGE, {124, 76, 59} },
  { CZ_BLUE,   {65, 110, 106} },
  { CZ_GREEN,  {67, 59, 44} }
};

static const int REFERENCE_COUNT = sizeof(REFERENCES) / sizeof(REFERENCES[0]);

/* ****************************************************************************
Color detection
**************************************************************************** */
enum Color_Zone detect_color(const uint16_t color_values[]) {
    // Seuil ramené à 45 pour éviter que l'ombre du plancher 
    // ne s'approche trop des valeurs sombres du bleu ou du vert
    const uint16_t max_color_distance = 45; 

    const uint64_t max_distance_squared =
        static_cast<uint64_t>(max_color_distance) * max_color_distance;

    uint64_t min_distance_squared = 0xFFFFFFFFFFFFFFFFULL;
    enum Color_Zone detected_zone = CZ_OTHER;

    for (int i = 0; i < REFERENCE_COUNT; i++) {
        int32_t red_difference =
            static_cast<int32_t>(color_values[0]) - REFERENCES[i].rgb[0];
        int32_t green_difference =
            static_cast<int32_t>(color_values[1]) - REFERENCES[i].rgb[1];
        int32_t blue_difference =
            static_cast<int32_t>(color_values[2]) - REFERENCES[i].rgb[2];

        uint64_t distance_squared =
            static_cast<uint64_t>(static_cast<int64_t>(red_difference) * red_difference) +
            static_cast<uint64_t>(static_cast<int64_t>(green_difference) * green_difference) +
            static_cast<uint64_t>(static_cast<int64_t>(blue_difference) * blue_difference);

        if (distance_squared < min_distance_squared) {
            min_distance_squared = distance_squared;
            detected_zone = REFERENCES[i].zone;
        }
    }

    if (min_distance_squared > max_distance_squared) {
        return CZ_OTHER;
    }

    return detected_zone;
}

/* ****************************************************************************
Sensor initialisation and reading
**************************************************************************** */
void InitColorSensor() {
    if (!colorSensor.begin()) {
        Serial.println("Color sensor not detected. Check its I2C wiring.");
        while (true) {
            delay(1000);
        }
    }
}

enum Color_Zone GetZoneColor() {
    uint16_t red, green, blue, clear;
    colorSensor.getRawData(&red, &green, &blue, &clear);

    // DÉBOGAGE : Décommentez ces deux lignes pour lire la valeur 'clear' sur le plancher
    // Serial.print("Clear : ");
    // Serial.println(clear);

    // Filtrage du plancher par intensité lumineuse
    // Le plancher réfléchit la lumière de la LED blanche très fortement.
    // Si la valeur est supérieure à 250, c'est le plancher.
    if (clear > 250) {
        return CZ_OTHER; 
    }

    uint16_t color_values[] = {red, green, blue};
    return detect_color(color_values);
}