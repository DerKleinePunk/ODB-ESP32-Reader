#pragma once
#include "Arduino.h"

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

    String GetCommand();
};


