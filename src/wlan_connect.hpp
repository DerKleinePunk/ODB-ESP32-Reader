#pragma once

class WlanConnect
{
private:
    const char* _ssid;
    const char* _password;
public:
    WlanConnect(const char* ssid, const char* password);
    ~WlanConnect();

    bool Init();
    bool Check();
};


