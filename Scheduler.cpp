#include "Scheduler.h"
#include "Plotter.h"
#include "Parser.h"
#include "Input.h"

namespace scheduler {
    MoveState moveState = IDLE;

    void tick() {
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
                    parser::startNextMovement();
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
        
        // refill parser queue
        if (parser::hasQueueSpace()) {
            if (input::lineReady()) {
                parser::addLine(input::takeLine());
                //input.putLine(parser::readyLine(), parser::getLineLength());
            }
        }
        
        // read from serial
        if (!input::lineReady()) {
            input::poll();
        }
        //if (!input::lineReady() && input::isReading()) {
        //    input::pollSerial();
        //}
    
        /*
        switch (currentState) {
            case IDLE:
                input::pollInput();
                
                if (input::isReading()) {
                    currentState = READING;
                }
                break;
            case MOVING:
                plotter::updateMovement();
                
                // check if we finish 
                if (!plotter::isMoving()) {
                
                }
                break;
            case MOVING_READING:
            
                break;
            case MOVING_PARSING:
            
                break;
            case MOVING_READING_PARSING
            case READING:
            
                break;
            case PARSING:
            
                break;
            default:
            
                break;
        }
                */
    }
}
