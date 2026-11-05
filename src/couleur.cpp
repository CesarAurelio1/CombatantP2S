#include <Arduino.h>
#include "couleur.h"
#include <Adafruit_TCS34725.h>

/* ****************************************************************************
Sensor object
**************************************************************************** */
// The sensor is created ONCE as a global object (not inside the function that
// reads it). Creating it on every call would re-initialise the I2C device each
// time, which is slow and unnecessary.
// Integration time = how long the sensor collects light per reading
// (longer = more precise but slower). Gain = signal amplification (16x here).
static Adafruit_TCS34725 colorSensor(
    TCS34725_INTEGRATIONTIME_2_4MS,
    TCS34725_GAIN_16X
);

/* ****************************************************************************
Reference color table
**************************************************************************** */
// Each entry links a zone (enum) to the raw {R, G, B} values the sensor gave us
// when it was placed over that color. Using the enum directly in the table means
// we no longer have to remember that "index 2 = red": the table says it itself.
struct ColorReference {
  enum Color_Zone zone;      // The zone this entry represents
  uint16_t rgb[3];           // Manually measured values {red, green, blue}
};

// Adjust these values after placing the sensor correctly on the robot.
static const ColorReference REFERENCES[] = {
  { CZ_RED,    {210, 158, 133} },   // Measured on the robot (average)
  { CZ_YELLOW, {165, 187, 97} },    // Measured on the robot (average)
  { CZ_ORANGE, {130, 113, 69} },    // Old value (only the proportions matter)
  { CZ_BLUE,   {70, 123, 119} },    // Measured on the robot
  { CZ_GREEN,  {124, 182, 142} },   // Measured on the robot (average)
  // White floor: not a zone, but it MUST be in the table. Without it, white is
  // matched to the closest color (it used to be green).
  { CZ_OTHER,  {102, 113, 90} },    // Measured on the robot (average)
};

// Set to true to print "R G B -> zone" on every reading (for calibration).
// Set back to false afterwards: printing at 9600 baud slows the line follower.
static const bool PRINT_COLOR_VALUES = false;

// Number of entries in the table (computed automatically, so adding a color
// to REFERENCES does not require changing the loop below).
static const int REFERENCE_COUNT = sizeof(REFERENCES) / sizeof(REFERENCES[0]);

/* ****************************************************************************
Color detection
**************************************************************************** */
// Converts raw {R, G, B} into proportions: each channel as a share (per
// thousand) of R + G + B. When the sensor is higher or lower, all three raw
// values go up or down together but the proportions stay the same. Comparing
// raw values made a darker yellow look like orange.
static void to_proportions(const uint16_t rgb[3], int32_t proportions[3]) {
    int32_t sum = static_cast<int32_t>(rgb[0]) + rgb[1] + rgb[2];
    if (sum == 0) sum = 1; // Avoid dividing by zero

    for (int channel = 0; channel < 3; channel++) {
        proportions[channel] = static_cast<int32_t>(rgb[channel]) * 1000 / sum;
    }
}

enum Color_Zone detect_color(const uint16_t color_values[]) {
    // Below this brightness (R + G + B) the proportions are too noisy to trust
    // (e.g. black line). The darkest measured surfaces (white, blue) are ~300.
    const int32_t min_brightness = 150;

    // Largest distance (in proportion units, per thousand) we still accept as
    // a match. Closest references (red / orange) are 64 apart, hence 30.
    const int32_t max_color_distance = 30;

    // We compare SQUARED distances to avoid computing a square root (slow on
    // an Arduino). So the threshold must be squared too.
    // Proportions are at most 1000, so 32-bit math is enough here.
    const int32_t max_distance_squared = max_color_distance * max_color_distance;

    if (static_cast<int32_t>(color_values[0]) + color_values[1] + color_values[2]
            < min_brightness) {
        return CZ_OTHER;
    }

    int32_t measured[3];
    to_proportions(color_values, measured);

    // Start with the biggest possible value so the first comparison always wins.
    int32_t min_distance_squared = INT32_MAX;

    // Best match so far. Defaults to CZ_OTHER ("unknown").
    enum Color_Zone detected_zone = CZ_OTHER;

    // Go through each reference color and keep the closest one.
    for (int i = 0; i < REFERENCE_COUNT; i++) {
        int32_t reference[3];
        to_proportions(REFERENCES[i].rgb, reference);

        // Difference between the measured value and the reference, per channel.
        int32_t red_difference = measured[0] - reference[0];
        int32_t green_difference = measured[1] - reference[1];
        int32_t blue_difference = measured[2] - reference[2];

        // 3D distance formula (Pythagoras) without the square root:
        // d^2 = dR^2 + dG^2 + dB^2
        int32_t distance_squared =
            red_difference * red_difference +
            green_difference * green_difference +
            blue_difference * blue_difference;

        // Keep this reference if it is closer than the previous best.
        if (distance_squared < min_distance_squared) {
            min_distance_squared = distance_squared;
            detected_zone = REFERENCES[i].zone;
        }
    }

    // Even the closest reference is too far away: we don't recognise the color.
    if (min_distance_squared > max_distance_squared) {
        return CZ_OTHER;
    }

    return detected_zone;
}

/* ****************************************************************************
Sensor initialisation and reading
**************************************************************************** */
void InitColorSensor() {
    // begin() returns false if the sensor does not answer on the I2C bus.
    if (!colorSensor.begin()) {
        Serial.println("Color sensor not detected. Check its I2C wiring.");
        // Nothing useful can be done without the sensor: halt here.
        while (true) {
            delay(1000);
        }
    }
}

enum Color_Zone GetZoneColor() {
    uint16_t red, green, blue, clear;

    // Read the raw values from the sensor. "clear" is the total light
    // without a color filter; we don't use it but the function requires it.
    colorSensor.getRawData(&red, &green, &blue, &clear);

    // Pack the three channels in an array and find the closest zone.
    uint16_t color_values[] = {red, green, blue};
    enum Color_Zone zone = detect_color(color_values);

    // Zone printed as the enum value: 0 = RED, 1 = YELLOW, 2 = ORANGE,
    // 3 = BLUE, 4 = GREEN, 5 = OTHER.
    if (PRINT_COLOR_VALUES) {
        Serial.print(red);   Serial.print(" ");
        Serial.print(green); Serial.print(" ");
        Serial.print(blue);  Serial.print(" -> ");
        Serial.println(zone);
    }

    return zone;
}