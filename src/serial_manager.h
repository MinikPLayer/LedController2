#include <Arduino.h>
#include "parser.h"

static const char _SERIAL_MANAGER_SKIP_CHARACTERS[] = { '\r' };

template<int T>
class SerialManager {
    char buffer[T];
    int currentBufferIndex = 0;

    static bool isSkipped(char c) {
        for(uint i = 0; i < sizeof(_SERIAL_MANAGER_SKIP_CHARACTERS); i++) {
            if(_SERIAL_MANAGER_SKIP_CHARACTERS[i] == c) {
                return true;
            }
        }

        return false;
    }
public:
    void resetBuffer() {
        for(int i = 0; i < T; i++) {
            buffer[i] = 0;
        }
        currentBufferIndex = 0;
    }

    void setup(int baudRate) {
        Serial.begin(baudRate);

        resetBuffer();
    }

    void loop() {
        int availableSerial = Serial.available();
        if(availableSerial > 0) {
            for(int i = 0; i < availableSerial; i++) {
                // T - 1 to leave space for the null terminator
                if (currentBufferIndex >= T - 1) {
                    Serial.print("ERROR: Packet too large for the Serial buffer. Max size: ");
                    Serial.println(T);
                    resetBuffer();
                }

                char c = Serial.read();
                if(c == '\n') {
                    Parser::ParseCommand(buffer);
                    resetBuffer();
                }
                else {
                    if(!isSkipped(c)) {
                        buffer[currentBufferIndex++] = c;
                    }
                }


            }
        }
    }
};