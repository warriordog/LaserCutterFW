#include "Input.h"
#include <Arduino.h>
#include "Config.h"
#include "LaserFW.h"
#include "GCode.h"
#include "Plotter.h"
#include "Laser.h"
#include "Safety.h"

namespace input {
    // 1 extra space for null byte
    char buffer[MAX_GCODE_LENGTH + 1];
    int nextBufferIdx = 0;
    BufferState bufferState = EMPTY;
    
    // if true, commands are blocked until queue is empty
    bool waitForEmpty = false;
    
    // state of the immediate handler
    ImmHandlerState immState = NONE;

    void setup() {
        Serial.begin(SERIAL_BAUD);
        sendLine(F(FIRMWARE_VERSION));
    }

    void sendLine(const __FlashStringHelper* message) {
        sendMessage(message);
        sendNewline();
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
    
    void sendOK() {
        sendMessage(F("OK\n"));
    }
    
    void sendNewline() {
        sendChar('\n');
    }
    
    
    void poll() {
        if (Serial.available() > 0) {
            if (updateImmediateHandler(Serial.peek())) {
                //ignore byte handled by immediate handler
                Serial.read();
            // don't read if we are emptying buffer or if the buffer is full
            } else if (!waitForEmpty && bufferState != READY) {
                int ch = Serial.read();
                
                // lock buffer when newline received (and don't add newline)
                if (ch == '\n') {
                    // add null byte to end of input (unless buffer is full)
                    if (nextBufferIdx < MAX_GCODE_LENGTH) {
                        buffer[nextBufferIdx] = 0;
                    }
                    
                    nextBufferIdx = 0;
                    bufferState = READY;
                    
                    sendOK();
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
    
    void immWriteBack(char ch1, char ch2, char ch3, char ch4) {
        immWriteBack(ch1);
        immWriteBack(ch2);
        immWriteBack(ch3);
        immWriteBack(ch4);
    }
    
    bool updateImmediateHandler(int ch) {
        switch (immState) {
            case NONE:
                if (ch == 'M') {
                    immState = M;
                    return true;
                } else if (ch == 'I') {
                    immState = I;
                    return true;
                }
                break;
            case M:
                if (ch == '1') {
                    immState = M1;
                    return true;
                } else if (ch == '4') {
                    immState = M4;
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
                    immState = M112;
                    return true;
                } else {
                    immWriteBack('M', '1', '1');
                    immState = NONE;
                }
                break;
            // M112 emergency stop
            case M112:
                if (ch == '\n') {
                    sendOK();
                    shutdownMachine();
                    immState = NONE;
                    return true;
                } else {
                    immWriteBack('M', '1', '1', '2');
                    immState = NONE;
                }
                break;
            case I:
                if (ch == '0') {
                    immState = I0;
                    return true;
                } else if (ch == '1') {
                    immState = I1;
                    return true;
                } else {
                    immWriteBack('I');
                    immState = NONE;
                }
                break;
            // I0 abort previous command
            case I0:
                if (ch == '\n') {
                    sendOK();
                    gcode::abortCurrentCommand();
                    immState = NONE;
                    return true;
                } else {
                    immWriteBack('I', '0');
                    immState = NONE;
                }
                break;
            // I1 immediate full status update
            case I1:
                if (ch == '\n') {
                    sendOK();
                    Serial.print(F("I1 X"));
                    Serial.print(plotter::getXLocation());
                    Serial.print(F(" Y"));
                    Serial.print(plotter::getYLocation());
                    Serial.print(F(" F"));
                    Serial.print(plotter::getXSpeed());
                    Serial.print(F(" P"));
                    Serial.print(laser::isLaserOn());
                    Serial.print(F(" S"));
                    Serial.print(laser::getLaserLevel());
                    Serial.print(F(" T"));
                    Serial.print(safety::isLaserSafetyEngaged());
                    sendNewline();
                    immState = NONE;
                    return true;
                } else {
                    immWriteBack('I', '1');
                    immState = NONE;
                }
                break;
            case M4:
                if (ch == '0') {
                    immState = M40;
                    return true;
                }else {
                    immWriteBack('M', '4');
                    immState = NONE;
                }
                break;
            case M40:
                if (ch == '0') {
                    immState = M400;
                    return true;
                } else {
                    immWriteBack('M', '4', '0');
                    immState = NONE;
                }
                break;
            // M400 - flush buffer
            case M400:
                if (ch == '\n') {
                    waitForEmpty = true;
                    
                    // write back command and send
                    immWriteBack('M', '4', '0', '0');
                    nextBufferIdx = 0;
                    bufferState = READY;
                    immState = NONE;
                    return true;
                } else {
                    immWriteBack('M', '4', '0', '0');
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
    
    void m114Finished() {
        sendOK();
        waitForEmpty = false;
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
        Serial.print(F("'\n"));
        
        Serial.print(F("input::nextBufferIdx="));
        Serial.print(nextBufferIdx);
        sendNewline();
        
        Serial.print(F("input::bufferState="));
        switch(bufferState) {
            case EMPTY: Serial.print(F("EMPTY\n")); break;
            case BUILDING: Serial.print(F("BUILDING\n")); break;
            case READY: Serial.print(F("READY\n")); break;
            case OVERFLOW: Serial.print(F("OVERFLOW\n")); break;
            default: {
                Serial.print(bufferState);
                sendNewline();
            }
        }
    }
}
