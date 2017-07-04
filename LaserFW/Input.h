#ifndef INPUT_H
#define INPUT_H
#include <Arduino.h>

namespace input {
    enum BufferState {
        // line buffer is empty
        EMPTY,
        
        // line buffer is being filled
        BUILDING,
        
        // line buffer is ready to be read
        READY,
        
        // line buffer overflowed
        OVERFLOW
    };
    
    enum ImmHandlerState {
        NONE,
        M,
        M1,
        M11,
        M112,
        I,
        I0,
        I1,
        M4,
        M40,
        M400
    };

    void setup();
    
    void sendLine(const __FlashStringHelper* message);
    void sendMessage(const __FlashStringHelper* message);
    void sendString(char* str);
    void sendInt(int val);
    void sendLong(long val);
    void sendULong(unsigned long val);
    void sendBool(bool val);
    void sendChar(char chr);
    void sendOK();
    void sendNewline();

    bool lineReady();
    void poll();
    String* takeLine();
    
    bool updateImmediateHandler(int chr);
    
    void m114Finished();
    
    void printDebug();
}

#endif //INPUT_H
