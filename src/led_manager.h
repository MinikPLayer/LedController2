#include <Arduino.h>
#include <FastLED.h>

#include "config.h"

class ILedStrip {
public:
    virtual CRGB get(size_t index) = 0;
    virtual void set(size_t index, CRGB color) = 0;
};

template<template<uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, uint8_t DATA_PIN, size_t LEDS_COUNT, EOrder RGB_ORDER>
class LedStrip : public ILedStrip {
    CRGB leds[LEDS_COUNT];

public:
    CRGB get(size_t index) {
        if (index < 0 || index >= LEDS_COUNT) {
            LOG_ERROR("Index out of bounds: ", index);
            return CRGB::Red;
        }

        return leds[index];
    }

    void set(size_t index, CRGB color) {
        if (index < 0 || index >= LEDS_COUNT) {
            LOG_ERROR("Index out of bounds: ", index);
            return;
        }

        leds[index] = color;
    }

    LedStrip() {
        FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>(leds, LEDS_COUNT);
    }
};

template<size_t N>
class LedManager {
    ILedStrip* ledStrips[N];

public:
    void set(size_t index, CRGB color, size_t strip = 0) {
        if (strip < 0 || strip >= N) {
            LOG_ERROR("Strip index out of bounds: ", strip);
            return;
        }

        ledStrips[strip]->set(index, color);
    }

    CRGB get(size_t index, size_t strip = 0) {
        if (strip < 0 || strip >= N) {
            LOG_ERROR("Strip index out of bounds: ", strip);
            return CRGB::Red;
        }

        return ledStrips[strip]->get(index);
    }

    LedManager(std::initializer_list<ILedStrip*> ledStrips) {
        if (ledStrips.size() != N) {
            LOG_ERROR("Invalid number of led strips: ", ledStrips.size());
            return;
        }

        size_t i = 0;
        for(auto* ledStrip : ledStrips) {
            this->ledStrips[i++] = ledStrip;
        }
    }
};