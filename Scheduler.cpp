#include "Scheduler.h"
#include "Plotter.h"
#include "Parser.h"
#include "Input.h"

namespace scheduler {

    void tick() {
        //Serial.println(F("SchedulerTick"));
        // update movement
        if (plotter::isMoving()) {
        
            //Serial.println(F("Plotter moving"));
            plotter::updateMovement();
        // start next task
        } else if (parser::hasWork()) {
            //Serial.println(F("Parser working"));
            parser::startNextCode();
        }
        
        // refill parser queue
        if (parser::hasQueueSpace()) {
            if (input::lineReady()) {
                parser::addLine(input::takeLine());
            }
        }
        
        // read from serial
        if (!input::lineReady()) {
            input::poll();
        }
    }
    
    /*
    void printDebug() {
        input::sendMessage(F("scheduler::moveState="));
        switch(moveState) {
            case IDLE: input::sendMessage(F("IDLE\n")); break;
            case MOVING: input::sendMessage(F("MOVING\n")); break;
            case EXECUTING: input::sendMessage(F("EXECUTING\n")); break;
            default: input::sendInt(moveState); input::sendChar('\n');
        }
    }
    */
}
