#include "Parser.h"
#include <Arduino.h>
#include "Config.h"
#include <QueueArray.h>
#include "GCode.h"
#include "Input.h"

namespace parser {

    QueueArray<String*> lineQueue;
      
    int skipSpace(int start, String* line) {
        while (start < line->length() && line->charAt(start) == ' ') {
            start++;
        }
        return start;
    }
    
    // stops at end
    int skipToSpace(int start, String* line) {
        while (start < line->length() && line->charAt(start) != ' ') {
            start++;
        }
        return start;
    }
    
    void parseLine(String* line) {
        gcode::Field* command = nullptr;
        gcode::FieldList* args = new gcode::FieldList;
        
        int idx = 0;
        while (idx < line->length()) {
        
            char c = line->charAt(idx);
            
            // skip spaces
            if (c == ' ') {
                idx++;
                continue;
            }
        
            // stop when we hit a comment
            if (c == ';') {
                break;
            }
            
            // convert to upper case
            if (c >= 'a' && c <= 'z') {
                c -= ('a' - 'A');
            }
            
            // skip non-letters
            if (c < 'A' || c > 'Z') {
                idx = skipToSpace(idx, line);
                continue;
            }
            
            // skip line numbers and checksums
            if (c == 'N' || c == '*') {
                idx = skipToSpace(idx, line);
                continue;
            }
            
            char letter = c;
            int start = idx + 1;
            int end = skipToSpace(start, line);
            
            // make sure the numbers are in bounds
            if (start < line->length()) {
                long iNum = line->substring(start, end).toInt();
                float fNum = line->substring(start, end).toFloat();
                gcode::Field* field = new gcode::Field;
                field->letter = letter;
                field->iNum = iNum;
                field->fNum = fNum;
            
                // store as command or as arg
                if (command == nullptr) {
                    command = field;
                } else {
                    args->add(field);
                }
            }
            
            // move to next code
            idx = end;
        }
        
        if (command == nullptr) {
            delete args;
        } else {
            gcode::execute(command, args);
        }
    }
    
    bool hasWork() {
        return !lineQueue.isEmpty() || gcode::isWorking();
    }
    
    void startNextCode() {
        //Serial.println(F("Starting movement"));
        if (gcode::isWorking()) {
            gcode::update();
        } else if (!lineQueue.isEmpty()) {
            String* line = lineQueue.pop();
            parseLine(line);
            delete line;  // no memory leaks
        }
    }
    
    bool hasQueueSpace() {
        return lineQueue.count() < GCODE_QUEUE_SIZE;
    }
    
    void clearBuffer() {
        while (!lineQueue.isEmpty()) {
            delete lineQueue.pop();
        }
    }
    
    void addLine(String* string) {
        if (string != nullptr) {
        //Serial.println(F("Line added"));
            lineQueue.push(string);
        } else {
        //Serial.println(F("Line null"));
        }
    }
    
    void printDebug() {
        QueueArray<String*> temp;
        while (!lineQueue.isEmpty()) {
            temp.push(lineQueue.pop());
        }
    
        input::sendMessage(F("parser::lineQueue="));
        input::sendInt(temp.count());
        input::sendChar('[');
        
        bool first = true;
        while (!temp.isEmpty()) {
            if (!first) {
                input::sendChar(',');
            }
            first = false;
        
            String* line = temp.pop();
            if (line == nullptr) {
                input::sendMessage(F("nullptr"));
            } else {
                input::sendString(line->c_str());
            }
            
            lineQueue.push(line);
        }
        
        input::sendMessage(F("]\nparser::hasWork()="));
        input::sendBool(hasWork());
        input::sendChar('\n');
    }
}
