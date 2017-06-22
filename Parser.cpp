#include "Parser.h"
#include <Arduino.h>
#include "Config.h"
#include <QueueArray.h>
#include "GCode.h"

namespace parser {

    QueueArray<String*> lineQueue;
      
    int skipSpace(int start, String* line) {
        while (start < line->length() && line[start] == ' ') {
            start++;
        }
        return start;
    }
    
    int skipToSpace(int start, String* line) {
        return line->indexOf(' ', start);
    }
    
    void parseLine(String* line) {
        gcode::Field* command = nullptr;
        gcode::FieldList* args = new gcode::FieldList;
        
        int idx = 0;
        while (idx < line->length()) {
            // skip spaces
            idx = skipSpace(idx, line);
        
            char c = line->charAt(idx);
        
            // stop when we hit a comment
            if (c == ';') {
                break;
            }
            
            // convert to upper case
            if (c >= 'a' && c <= 'z') {
                c -= ('a' - 'A');
            }
            
            // skip missing letters
            if (c <= 'A' && c >= 'Z') {
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
            if (start < line->length() - 1 && end - start > 0) {
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
    
    void startNextMovement() {
        if (gcode::isWorking()) {
            gcode::update();
        } else if (!lineQueue.isEmpty()) {
            String* line = lineQueue.pop();
            parseLine(line);
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
}
