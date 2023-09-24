#include "mqtt_connect.hpp"


MqttConnect::MqttConnect()
{
}

MqttConnect::~MqttConnect()
{
}

void MqttConnect::Search()
{
    if(mdns_init()!= ESP_OK) {
        log_e("mDNS failed to start");
        return;
    }

    int nrOfServices = MDNS.queryService("mqtt", "tcp");
   
    if (nrOfServices == 0) {
        Serial.println("No mqtt service were found.");
    } else {
        Serial.print("Number of services found: ");
        Serial.println(nrOfServices);
        
        for (int i = 0; i < nrOfServices; i=i+1) {
    
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