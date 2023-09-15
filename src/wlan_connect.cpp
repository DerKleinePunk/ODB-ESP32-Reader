#include "wlan_connect.hpp"
#include <WiFi.h>


WlanConnect::WlanConnect(const char* ssid, const char* password)
{
    _ssid = ssid;
    _password = password;
}

WlanConnect::~WlanConnect()
{
}

bool WlanConnect::Init()
{
    auto count = 0;
    // Wi-Fi connection
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        count++;
        if(count == 20) {
            break;
        }
    }

    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");

        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("WiFi not connected!!");
    }

    return false;
}

bool WlanConnect::Check()
{
    if(WiFi.status() == WL_CONNECTED) {
        return true;
    }

    Serial.println("WiFi not connected!!");
    
    return false;
}