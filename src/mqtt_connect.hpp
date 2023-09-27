#pragma once

#include <ESPmDNS.h>
#include <MQTT.h>

class MqttConnect
{
private:
    IPAddress _ipAdress;
public:
    MqttConnect();
    ~MqttConnect();

    void begin(Client& _client);
    void Search();
    bool Connect(String& deviceId);
    bool loop();
    bool publish(const std::string& topic, const std::string& payload);
};


