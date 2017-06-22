#include "Scheduler.h"
#include "Plotter.h"
#include "Parser.h"
#include "Input.h"

namespace scheduler {
    MoveState moveState = IDLE;

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
    
    /*
        // update movement and exeuction
        switch (moveState) {
            case IDLE:
            
                break;
            case EXECUTING:
                // current movement is last one
                if (!parser::hasWork()) {
                    // drop to "moving" state
                    if (plotter::isMoving()) {
                        moveState = MOVING;
                    // finished
                    } else {
                        moveState = IDLE;
                    }
                // if we have finished the current movement but there is more to do
                } else if (!plotter::isMoving()) {
                    parser::startNextCode();
                }
                break;
            case MOVING:
                plotter::updateMovement();
                
                if (!plotter::isMoving()) {
                    moveState = IDLE;
                }
                break;
            default:
                // this should not happen
                moveState = IDLE;
                break;
        }
        */
        
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
}
