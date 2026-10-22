#include <Arduino.h>
#include <couleur.h>
#include <Adafruit_TCS34725.h>

int detect_color(uint16_t color_values[]) {
    const uint16_t orange_values[] = {5613, 4985, 3242};
    const uint16_t yellow_values[] = {8280, 8951, 4827};
    const uint16_t red_values[]    = {9565, 6866, 5800};
    const uint16_t green_values[]  = {7081, 10110, 7925};
    const uint16_t blue_values[]   = {3770, 6375, 6211};

    const uint16_t* reference_values[] = {
        orange_values, yellow_values, red_values,
        green_values, blue_values
    };

    uint64_t min_distance_squared = 0xFFFFFFFFFFFFFFFFULL;
    int detected_color = -1;

    const uint16_t max_color_distance = 2000; // Adjust this threshold while testing.
    const uint64_t max_distance_squared =
        static_cast<uint64_t>(max_color_distance) * max_color_distance;

    for (int i = 0; i < 5; i++) {
        int32_t red_difference =
            static_cast<int32_t>(color_values[0]) - reference_values[i][0];
        int32_t green_difference =
            static_cast<int32_t>(color_values[1]) - reference_values[i][1];
        int32_t blue_difference =
            static_cast<int32_t>(color_values[2]) - reference_values[i][2];
        uint64_t distance_squared =
            static_cast<int64_t>(red_difference) * red_difference +
            static_cast<int64_t>(green_difference) * green_difference +
            static_cast<int64_t>(blue_difference) * blue_difference;

        if (distance_squared < min_distance_squared) {
            min_distance_squared = distance_squared;
            detected_color = i;
        }
    }

    if (min_distance_squared > max_distance_squared) {
        return -1; // No sufficiently close match
    }

    return detected_color;
}

void couleur()
{
    Adafruit_TCS34725 colorSensor(
    TCS34725_INTEGRATIONTIME_154MS,
    TCS34725_GAIN_16X
);
    if (!colorSensor.begin())
    {
        Serial.println("Color sensor not detected. Check its I2C wiring.");
        while (true)
        {
            delay(1000);
        }
    }

    while (true)
    {
        uint16_t red, green, blue, clear;
        colorSensor.getRawData(&red, &green, &blue, &clear);
        delay(100);
        //Serial.print("The RGB value are: RGB( ");
        //Serial.print(red);
        //Serial.print(", ");
        //Serial.print(green);
        //Serial.print(", ");
        //Serial.print(blue);
        //Serial.println(" )");

        uint16_t color_values[] = {red, green, blue};
        int detected_color = detect_color(color_values);
        Serial.println(detected_color);
    }
}