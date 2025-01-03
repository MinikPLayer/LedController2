#include <Arduino.h>
#include "parser.h"
#include "input_manager.h"
#include "config.h"

template<int T>
class SerialManager : public InputManager<T> {
    int baudRate;

    int availableCharsCount() override {
        return Serial.available();
    }

    char readChar() override {
        return Serial.read();
    }


public:

    void print(const char* data) override {
        Serial.print(data);
    }

    void print(int n) override {
        Serial.print(n);
    }

    void setup() override {
        InputManager<T>::setup();

        Serial.begin(baudRate);
        Serial.println("");
        LOG("SerialManager started!");
    }

    SerialManager(int baudRate) {
        this->baudRate = baudRate;
    }
};