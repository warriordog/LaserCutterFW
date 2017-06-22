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
        M11
    };

    void setup();

    void sendMessage(const __FlashStringHelper* message);
    void sendInt(int val);
    void sendChar(char chr);

    bool lineReady();
    void poll();
    String* takeLine();
    
    bool updateImmediateHandler(int chr);
}

#endif //INPUT_H
