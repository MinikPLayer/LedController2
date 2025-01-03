#pragma once
#include <string.h>

struct ConstString {
    int _length;
    const char* _data;
public:

    int length() {
        return _length;
    }

    const char* get_raw() {
        return _data;
    }

    void serial_print() {
        Serial.write(_data, _length);
    }

    void serial_println() {
        Serial.write(_data, _length);
        Serial.println();
    }

    bool operator==(const char* other) {
        return strncmp(_data, other, _length) == 0;
    }

    ConstString() {
        this->_data = nullptr;
        this->_length = 0;
    }

    ConstString(const char* data, int length) {
        this->_data = data;
        this->_length = length;
    }

    ConstString(const char* data) {
        this->_data = data;
        this->_length = strlen(data);
    }
};