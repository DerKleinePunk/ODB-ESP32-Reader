#include "command_reader.hpp"

CommandReader::CommandReader(/* args */)
{
    _readBufferPos = 0;
}

CommandReader::~CommandReader()
{
}

bool CommandReader::begin(Stream &stream)
{
    _pInputStream = &stream;
    return true;
}

String CommandReader::GetCommand()
{
    String result("");

    while (_pInputStream->available() > 0) {
        int input = _pInputStream->read();
        if(input == 13) {
            _readBuffer[_readBufferPos] = 0;
            result = _readBuffer;
            _readBufferPos = 0;
        } else if (input == 10) {
            //ignore it
        } else {
            _readBuffer[_readBufferPos] = (char)input;
            _readBufferPos++;
        }
    }

    return result;
}