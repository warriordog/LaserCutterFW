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


    void sendMessage(const __FlashStringHelper* message) {
        Serial.print(message);
    }
    
    void sendInt(int val) {
        Serial.print(val);
    }
    
    void sendChar(char chr) {
        Serial.print(chr);
    }
    
    void sendString(char* str) {
        Serial.print(str);
    }
    
    void sendLong(long val) {
        Serial.print(val);
    }
    
    void sendULong(unsigned long val) {
        Serial.print(val);
    }
    
    void sendBool(bool val) {
        Serial.print(val);
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
                    
                    /*
                    Serial.print(F("Ready: '"));
                    Serial.print(buffer);
                    Serial.println('\'');
                    */
                    
                    sendMessage(F("OK\n"));
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
        //Serial.print(F("Taking: "));
        //Serial.println(str->c_str());
        nextBufferIdx = 0;
        bufferState = EMPTY;
        return str;
    }
    
    void immWriteBack(char ch1) {
        if (nextBufferIdx < MAX_GCODE_LENGTH) {
            buffer[nextBufferIdx] = ch1;
            nextBufferIdx++;
        }
    }
    
    void immWriteBack(char ch1, char ch2) {
        immWriteBack(ch1);
        immWriteBack(ch2);
    }
    
    void immWriteBack(char ch1, char ch2, char ch3) {
        immWriteBack(ch1);
        immWriteBack(ch2);
        immWriteBack(ch3);
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
                    immWriteBack('M');
                    immState = NONE;
                }
                break;
            case M1:
                if (ch == '1') {
                    immState = M11;
                    return true;
                } else {
                    immWriteBack('M', '1');
                    immState = NONE;
                }
                break;
            case M11:
                if (ch == '2') {
                    sendMessage(F("OK\n"));
                    shutdownMachine();
                    return true;
                } else {
                    immWriteBack('M', '1', '1');
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
    
    void printDebug() {
        //create a temp buffer with a null byte in case too big
        char temp[MAX_GCODE_LENGTH + 2];
        for (int i = 0; i <= MAX_GCODE_LENGTH; i++) {
            temp[i] = buffer[i];
        }
        temp[MAX_GCODE_LENGTH + 1] = 0;
        
        Serial.print(F("input::buffer="));//
        Serial.print(strlen(temp));
        Serial.print(F("/'"));
        Serial.print(buffer);
        Serial.print(F("'\n'"));
        
        Serial.print(F("input::nextBufferIdx="));
        Serial.println(nextBufferIdx);
        
        Serial.print(F("input::bufferState="));
        switch(bufferState) {
            case EMPTY: Serial.print(F("EMPTY\n")); break;
            case BUILDING: Serial.print(F("BUILDING\n")); break;
            case READY: Serial.print(F("READY\n")); break;
            case OVERFLOW: Serial.print(F("OVERFLOW\n")); break;
            default: Serial.println(bufferState);
        }
    }
}
