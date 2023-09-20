#pragma once
#include "Arduino.h"
#include "vector"

class CommandReader
{
private:
    Stream* _pInputStream;
    char _readBuffer[30];
    byte _readBufferPos = 0;
public:
    CommandReader(/* args */);
    ~CommandReader();

    bool begin(Stream &stream);

    std::vector<String> GetCommand();
    std::vector<String> GetCommandTimeout(u_long timeOut);
};


