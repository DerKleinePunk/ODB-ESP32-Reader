#include "command_reader.hpp"

CommandReader::CommandReader(/* args */)
{
    _readBufferPos = 0;
}

CommandReader::~CommandReader()
{
}

bool CommandReader::begin(Stream& stream)
{
    _pInputStream = &stream;
    return true;
}

std::vector<String> CommandReader::GetCommand()
{
    std::vector<String> result;

    while(_pInputStream->available() > 0) {
        int input = _pInputStream->read();
        if(input == 13) {
            log_d("<cr> found");
            if(_readBufferPos == 0) continue;
            _readBuffer[_readBufferPos] = 0;
            result.push_back(_readBuffer);
            _readBufferPos = 0;
        } else if(input == 10) {
            // ignore it
            log_d("<lf> found");
        } else if(input < 32) {
            // ignore it
            log_d("<%d> found", input);
        } else {
            _readBuffer[_readBufferPos] = (char)input;
            _readBufferPos++;
        }
    }

    return result;
}

std::vector<String> CommandReader::GetCommandTimeout(u_long timeOut)
{
    std::vector<String> result;

    // Todo Not Work after 50 Days run
    u_long now = millis();
    while((unsigned long)(millis() - now) < timeOut) {
        result = GetCommand();
        if(!result.empty()) {
            break;
        }
    }

    return result;
}