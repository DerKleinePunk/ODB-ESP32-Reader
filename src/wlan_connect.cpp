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
    WiFi.mode(WIFI_STA);
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
    WiFi.setAutoReconnect(false);

    _lastCheck = millis();
    return false;
}

bool WlanConnect::Check()
{
    if(WiFi.status() == WL_CONNECTED) {
        return true;
    }

    if(WiFi.status() != WL_DISCONNECTED) {
        log_d("WLAN Disconnect");
        WiFi.disconnect();
        WiFi._setStatus(WL_DISCONNECTED);
    }

    if((unsigned long)(millis() - _lastCheck) > 15000) {
        log_d("Try Reconnect");
        if(WiFi.reconnect()) {
            Serial.println("WiFi reconnected");
        } else {
            Serial.println("WiFi not connected");
        }
        _lastCheck = millis();
    }
 
    return false;
}

//Client Interface
int WlanConnect::connect(IPAddress ip, uint16_t port) {
    Check();
    return _netClient.connect(ip, port);
}

int WlanConnect::connect(const char *host, uint16_t port) {
    Check();
    return _netClient.connect(host, port);
}

size_t WlanConnect::write(uint8_t buffer) {
    Check();
    return _netClient.write(buffer);
}

size_t WlanConnect::write(const uint8_t *buf, size_t size)
{
    Check();
    return _netClient.write(buf,size);
}

int WlanConnect::available(){
    Check();
    return _netClient.available();
}

 int WlanConnect::read()
 {
    Check();
    return _netClient.read();
 }

 int WlanConnect::read(uint8_t *buf, size_t size)
 {
    Check();
    return _netClient.read(buf, size);
 }

 int WlanConnect::peek()
 {
    Check();
    return _netClient.peek();
 }

void WlanConnect::flush()
{
    Check();
    _netClient.flush();
}

void WlanConnect::stop()
{
    Check();
    _netClient.stop();
}

uint8_t  WlanConnect::connected()
{
    Check();
    return _netClient.connected();
}
