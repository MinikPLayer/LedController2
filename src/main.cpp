#include <FastLED.h>
#include <Arduino.h>
#include "parser.h"
#include "serial_manager.h"
#include "config.h"
#include "led_manager.h"

#define NUM_LEDS 18
#define DATA_PIN 15
#define DATA_PIN2 13

LedManager<2> leds({
    new LedStrip<WS2812B, DATA_PIN, NUM_LEDS, GRB>(),
    new LedStrip<WS2812B, DATA_PIN2, NUM_LEDS, GRB>()
});

#if ENABLE_ESP8266_WIFI
    #include "wifi8266_manager.h"

    auto* wifiManager = new Wifi8266Manager<256>();
#endif

InputManager<256>* managers[] = {
    new SerialManager<256>(115200),
    #if ENABLE_ESP8266_WIFI
        wifiManager
    #endif
};

void managersForEach(void (*func)(InputManager<256>*)) {
    for (size_t i = 0; i < sizeof(managers) / sizeof(managers[0]); i++) {
        func(managers[i]);
    }
}

void setup()
{
    managersForEach([](InputManager<256>* manager) {
        manager->setup();
    });

    LOG("Setup done!");

    for(int i = 0; i < NUM_LEDS; i++) {
        leds.set(i, CRGB::Red, 0);
        leds.set(i, CRGB::Red, 1);
    }
}

int c = 0;
int i = 0;

uint64_t lastLedUpdate = 0;
const uint64_t ledUpdateInterval = 150 * 1000;

void loop()
{
    managersForEach([](InputManager<256>* manager) {
        manager->loop();
    });

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
    CRGB secondColor;
    secondColor.setRGB(255 - actualColor.r, 255 - actualColor.g, 255 - actualColor.b);

    leds.set(i, actualColor, 0);
    leds.set(i, secondColor, 1);
    FastLED.show();
}