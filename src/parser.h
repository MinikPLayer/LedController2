#pragma once
#include "const_string.h"
#include "commands.h"

class Parser {
public:
    static const char ARGUMENT_SEPARATOR = '|';

    struct SplitResult {
        ConstString first;
        ConstString second;

        SplitResult() {}
        SplitResult(ConstString first, ConstString second) : first(first), second(second) {}
    };

    static SplitResult SplitArgs(ConstString args) {
        char* separator = strchr(args.get_raw(), ARGUMENT_SEPARATOR);
        if(separator == nullptr) {
            return SplitResult(args, ConstString());
        }

        return SplitResult(
            ConstString(args.get_raw(), separator - args.get_raw()),
            ConstString(separator + 1)
        );
    }

    static bool ParseCommand(const char* data) {
        auto result = SplitArgs(ConstString(data));

        if (result.first == "ping") {
            PingCommand::Run(result.second);
        }
        else {
            Serial.println("ERROR: Unknown command");
            return false;
        }

        return true;
    }
};