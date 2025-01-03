#include <FastLED.h>
#include <Arduino.h>
#include "parser.h"
#include "serial_manager.h"
#include "config.h"

#define NUM_LEDS 18
#define DATA_PIN 15
#define DATA_PIN2 13

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

SerialManager<256> serialManager(115200);

void setup()
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds2, NUM_LEDS);

    serialManager.setup();
    Serial.println("");
    Serial.print("[LEDController2] "); Serial.println(VERSION);
    Serial.println("OK");

    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = leds2[i] = CRGB::Red;
    }
}

int c = 0;
int i = 0;

uint64_t lastLedUpdate = 0;
const uint64_t ledUpdateInterval = 150 * 1000;

void loop()
{
    serialManager.loop();

    uint64_t now = micros64();
    if (now - lastLedUpdate >= ledUpdateInterval) {
        lastLedUpdate += ledUpdateInterval;

        i++;
        if (i >= NUM_LEDS) {
            i = 0;
            c++;
            if (c >= 3) {
                c = 0;
            }
        }
    }

    const CRGB COLORS[3] = {
        CRGB::Red,
        CRGB::Blue,
        CRGB::Green
    };

    CRGB color = COLORS[c];
    CRGB nextColor = COLORS[(c + 1) % 3];

    double blendFactor = 255.0 * (now - lastLedUpdate) / (double)ledUpdateInterval;
    CRGB actualColor = blend(color, nextColor, blendFactor);

    leds[i] = actualColor;
    leds2[i] = actualColor;
    FastLED.show();
}