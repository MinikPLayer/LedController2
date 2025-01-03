#include "config.h"

#if ENABLE_ESP8266_WIFI

#include "input_manager.h"
#include <ESP8266WiFi.h>

template<int T>
class Wifi8266Manager : public InputManager<T> {
    const char* ssid = nullptr;
    const char* pass = nullptr;

    WiFiClient client;

    int availableCharsCount() override {
        return client.available();
    }

    char readChar() override {
        return client.read();
    }

    bool isInitialized() {
        return ssid != nullptr && pass != nullptr;
    }
public:
    void print(const char* data) override {
        client.print(data);
    }

    void print(int n) override {
        client.print(n);
    }

    void setup() override {
        InputManager<T>::setup();

        if (!isInitialized()) {
            LOG_ERROR("Wifi8266Manager not initialized!");
            return;
        }

        auto status = WiFi.begin(ssid, pass);
        if (status != WL_CONNECTED) {
            LOG_ERROR("WiFi connection failed: ", status);
            return;
        }

        LOG("Connected to WiFi: ", WiFi.SSID());
    }

    void init(const char* ssid, const char* pass, bool setup = true) {
        this->ssid = ssid;
        this->pass = pass;

        if(setup) {
            this->setup();
        }
    }

    Wifi8266Manager(const char* ssid, const char* pass) {
        init(ssid, pass, false);
    }

    Wifi8266Manager() {}
};

#endif