#include "parser.h"
#include <Arduino.h>

class PingCommand {
public:
    static void Run(ConstString args) {
        // Parse the arguments
        Serial.print("Ping: ");
        args.serial_println();
    }
};