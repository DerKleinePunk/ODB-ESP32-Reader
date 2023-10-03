#ifdef MQTT_ENABLED

#include "mqtt_connect.hpp"

MQTTClient client;

MqttConnect::MqttConnect()
{
    _ipAdress = INADDR_NONE;
}

MqttConnect::~MqttConnect()
{
}

void MqttConnect::begin(Client& _client) {
    client.begin(_client);
}

void MqttConnect::Search()
{
    if(mdns_init() != ESP_OK) {
        log_e("mDNS failed to start");
        return;
    }

    int nrOfServices = MDNS.queryService("mqtt", "tcp");

    if(nrOfServices == 0) {
        Serial.println("No mqtt service were found.");
    } else {
        Serial.print("Number of services found: ");
        Serial.println(nrOfServices);

        for(int i = 0; i < nrOfServices; i++) {
            if(i == 0) {
                _ipAdress = MDNS.IP(i);
            }
            Serial.println("---------------");

            Serial.print("Hostname: ");
            Serial.println(MDNS.hostname(i));

            Serial.print("IP address: ");
            Serial.println(MDNS.IP(i));

            Serial.print("Port: ");
            Serial.println(MDNS.port(i));

            Serial.println("---------------");
        }
    }
}

bool MqttConnect::Connect(String& deviceId) 
{
    if(_ipAdress == INADDR_NONE) {
        log_e("no ip set");
        return false;
    }
    client.setHost(_ipAdress);

    return client.connect(deviceId.c_str());
}

bool MqttConnect::loop()
{
    return client.loop();
}

/// @brief send payload to mqtt
/// @param topic 
/// @param payload 
/// @return payload sendet
bool MqttConnect::publish(const std::string& topic, const std::string& payload)
{
    if(!client.connected()) {
        log_e("mqtt not connected");
        return false;
    }

    return client.publish(topic.c_str(), payload.c_str());
}

#endif