#include <Arduino.h>
#include "parser.h"
#include "input_manager.h"

template<int T>
class SerialManager : public InputManager<T> {
    int baudRate;

    int availableCharsCount() override {
        return Serial.available();
    }

    char readChar() override {
        return Serial.read();
    }

    void print(const char* data) override {
        Serial.print(data);
    }

    void print(int n) override {
        Serial.print(n);
    }

public:
    void onSetup() override {
        Serial.begin(baudRate);
    }

    SerialManager(int baudRate) {
        this->baudRate = baudRate;
    }
};