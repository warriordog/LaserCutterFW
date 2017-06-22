#include "Input.h"
#include <Arduino.h>
#include "Config.h"
#include "LaserFW.h"

namespace input {
    // 1 extra space for null byte
    char buffer[MAX_GCODE_LENGTH + 1];
    int nextBufferIdx = 0;
    BufferState bufferState = EMPTY;
    
    // state of the immediate handler
    ImmHandlerState immState = NONE;

    void setup() {
        Serial.begin(SERIAL_BAUD);
        Serial.println(F("LaserFW 0.0.1"));
    }

    bool lineReady() {
        return bufferState == READY;
    }
    
    void poll() {
        if (Serial.available() > 0) {
            if (updateImmediateHandler(Serial.peek())) {
                //ignore byte handled by immediate handler
                Serial.read();
            } else if (bufferState != READY) {
                int ch = Serial.read();
                
                // lock buffer when newline received (and don't add newline)
                if (ch == '\n') {
                    // add null byte to end of input (unless buffer is full)
                    if (nextBufferIdx < MAX_GCODE_LENGTH) {
                        buffer[nextBufferIdx] = 0;
                    }
                    
                    nextBufferIdx = 0;
                    bufferState = READY;
                // don't read into full buffer, just ignore the byte and wait for newline
                } else if (bufferState != OVERFLOW) {
                    buffer[nextBufferIdx] = ch;
                    nextBufferIdx++;
                    
                    // make sure buffer did not fill up
                    if (nextBufferIdx >= MAX_GCODE_LENGTH) {
                        nextBufferIdx = 0;
                        bufferState = OVERFLOW;
                    }
                }
            }
        }
    }
    
    String* takeLine() {
        String* str = new String(buffer);
        nextBufferIdx = 0;
        bufferState = EMPTY;
    }
    
    bool updateImmediateHandler(int ch) {
        switch (immState) {
            case NONE:
                if (ch == 'M') {
                    immState = M;
                    return true;
                }
                break;
            case M:
                if (ch == '1') {
                    immState = M1;
                    return true;
                } else {
                    immState = NONE;
                }
                break;
            case M1:
                if (ch == '1') {
                    immState = M11;
                    return true;
                } else {
                    immState = NONE;
                }
                break;
            case M11:
                if (ch == '2') {
                    shutdownMachine();
                    return true;
                } else {
                    immState = NONE;
                }
                break;
            default:
                // should not happen
                immState = NONE;
                break;
        }
        return false;
    }
}
