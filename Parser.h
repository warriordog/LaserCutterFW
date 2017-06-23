#ifndef PARSER_H
#define PARSER_H
#include <QueueArray.h>
#include <Arduino.h>

namespace parser {
    bool hasWork();
    void startNextCode();
    bool hasQueueSpace();
    void addLine(String* string); 
    void clearBuffer();
    
    void printDebug();
}

#endif //PARSER_H
