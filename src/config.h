#pragma once

#define VERSION "v0.0.1"

#define ENABLE_ESP8266_WIFI 1

template<typename ... Args>
void __log_func__(const char* type, const char* location, Args ... args) {
    Serial.print(type);
    Serial.print(" {");
    Serial.print(location);
    Serial.print("} ");

    (..., (Serial.print(args)));

    Serial.println();
}

#define LOG(...) __log_func__("[LOG]", __PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...) __log_func__("[ERROR]", __PRETTY_FUNCTION__, __VA_ARGS__)