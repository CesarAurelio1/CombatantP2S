#include <Arduino.h>
#include <couleur.h>
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
    TCS34725_INTEGRATIONTIME_154MS,
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
  { CZ_RED,    {10971, 8295, 6981} },
  { CZ_YELLOW, {9348, 10521, 5638} },
  { CZ_ORANGE, {8870, 7558, 4623} },
  { CZ_BLUE,   {3940, 6733, 6518} },
  { CZ_GREEN,  {6466, 9159, 7228} }
};

// Number of entries in the table (computed automatically, so adding a color
// to REFERENCES does not require changing the loop below).
static const int REFERENCE_COUNT = sizeof(REFERENCES) / sizeof(REFERENCES[0]);

/* ****************************************************************************
Color detection
**************************************************************************** */
enum Color_Zone detect_color(const uint16_t color_values[]) {
    // Largest distance (in raw sensor units) we still accept as a match.
    // Adjust this threshold while testing.
    const uint16_t max_color_distance = 2000;

    // We compare SQUARED distances to avoid computing a square root (slow on
    // an Arduino). So the threshold must be squared too.
    const uint64_t max_distance_squared =
        static_cast<uint64_t>(max_color_distance) * max_color_distance;

    // Start with the biggest possible value so the first comparison always wins.
    uint64_t min_distance_squared = 0xFFFFFFFFFFFFFFFFULL;

    // Best match so far. Defaults to CZ_OTHER ("unknown").
    enum Color_Zone detected_zone = CZ_OTHER;

    // Go through each reference color and keep the closest one.
    for (int i = 0; i < REFERENCE_COUNT; i++) {
        // Difference between the measured value and the reference, per channel.
        // Cast to signed 32-bit so the subtraction can be negative.
        int32_t red_difference =
            static_cast<int32_t>(color_values[0]) - REFERENCES[i].rgb[0];
        int32_t green_difference =
            static_cast<int32_t>(color_values[1]) - REFERENCES[i].rgb[1];
        int32_t blue_difference =
            static_cast<int32_t>(color_values[2]) - REFERENCES[i].rgb[2];

        // 3D distance formula (Pythagoras) without the square root:
        // d^2 = dR^2 + dG^2 + dB^2
        // 64-bit math is used because the squares can overflow 32 bits.
        uint64_t distance_squared =
            static_cast<uint64_t>(static_cast<int64_t>(red_difference) * red_difference) +
            static_cast<uint64_t>(static_cast<int64_t>(green_difference) * green_difference) +
            static_cast<uint64_t>(static_cast<int64_t>(blue_difference) * blue_difference);

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

    // Uncomment to see the raw values while calibrating the references.
     Serial.print("RGB( ");
     Serial.print(red);   Serial.print(", ");
     Serial.print(green); Serial.print(", ");
     Serial.print(blue);  Serial.println(" )");

    // Pack the three channels in an array and find the closest zone.
    uint16_t color_values[] = {red, green, blue};
    enum Color_Zone zone = detect_color(color_values);

    Serial.println(zone); // Testing only: prints the enum value (0 = CZ_RED, ...)

    return zone;
}