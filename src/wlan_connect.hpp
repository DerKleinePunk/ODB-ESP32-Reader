#pragma once

#include <WiFi.h>

class WlanConnect : public Client
{
private:
    const char* _ssid;
    const char* _password;
    WiFiClient _netClient;
public:
    WlanConnect(const char* ssid, const char* password);
    ~WlanConnect();

    bool Init();
    bool Check();

    //Client Interface
    int connect(IPAddress ip, uint16_t port);
    int connect(const char *host, uint16_t port);
    size_t write(uint8_t);
    size_t write(const uint8_t *buf, size_t size);
    int available();
    int read();
    int read(uint8_t *buf, size_t size);
    int peek();
    void flush();
    void stop();
    uint8_t connected();
    operator bool() {
        return Check();
    }
};


