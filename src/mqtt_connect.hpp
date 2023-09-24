#pragma once

#include <ESPmDNS.h>

class MqttConnect
{
private:
    /* data */
public:
    MqttConnect();
    ~MqttConnect();

    void Search();
    bool Connected();
};


