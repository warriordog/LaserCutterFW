#include "GCode.h"
#include "Plotter.h"
#include "Laser.h"
#include "Parser.h"
#include "LaserFW.h"
#include "Input.h"
#include "Lights.h"
#include "Scheduler.h"
#include "Safety.h"
#include "Config.h"

namespace gcode {
    // 0 is idle, 1 is code-default, everything else is code-specific
    long currState = 0;
    Field* currCommand = nullptr;
    FieldList* currArgs = nullptr;

    FieldList::~FieldList() {
        FieldNode* node = head;
        while (node != nullptr) {
            FieldNode* next = node->next;
            delete node->field;
            delete node;
            node = next;
        }
        
        // just in case
        head = nullptr;
    }
    
    void FieldList::add(Field* field) {
        if (head == nullptr) {
            head = new FieldNode;
            head->field = field;
        } else {
            for (FieldNode* node = head; node != nullptr; node = node->next) {
                if (node->next == nullptr) {
                    node->next = new FieldNode;
                    node->next->field = field;
                    
                    // if we don't break then the field will be added infinitely
                    break;
                }
            }
        }
    }
    
    void execute(Field* command, FieldList* args) {
        if (currCommand != nullptr) {
            delete currCommand;
        }
        if (currArgs != nullptr) {
            delete currArgs;
        }
    
        currCommand = command;
        currArgs = args;
        
        if (currCommand != nullptr) {
            currState = 1;
        }
    }
    
    bool isWorking() {
        return currState != 0;
    }
    
    void abortCurrentCommand() {
        execute(nullptr, nullptr);
        currState = 0;
    }
    
    Field* findField(char letter) {
        if (currArgs != nullptr) {
            for (FieldNode* node = currArgs->getHead(); node != nullptr; node = node->next) {
                if (node->field->letter == letter) {
                    return node->field;
                }
            }
        }
        return nullptr;
    }
    
    void update() {
        if (currState != 0) {
            // blink activity
            lights::blinkActivity();    
            
            switch (currCommand->letter) {
                // G-codes
                case 'G':
                    switch (currCommand->iNum) {
                        //G00 / G01 move
                        case 0:
                        case 1:
                            switch (currState) {
                                // start move
                                case 1:
                                    // if a previous movement is running, then wait for it to finish                                    
                                    if (plotter::isMoving()) {
                                        currState = 2;
                                    } else {
                                        Field* fF = findField('F');
                                        Field* fX = findField('X');
                                        Field* fY = findField('Y');
                                        
                                        if (fF != nullptr) {
                                            plotter::setXSpeed(fF->iNum);
                                            plotter::setYSpeed(fF->iNum);
                                        }
                                        
                                        if (fX != nullptr) {
                                            plotter::setTargetX(fX->iNum);
                                        }
                                        
                                        if (fY != nullptr) {
                                            plotter::setTargetY(fY->iNum);
                                        }
                                        
                                        currState = 3;
                                    }
                                    break;
                                // wait for previous move to finish
                                case 2:
                                    // check if previous move ended
                                    if (!plotter::isMoving()) {
                                        currState = 1;
                                    }
                                    break;
                                // wait for this move to finish
                                case 3:
                                default:
                                    // check if finished
                                    if (!plotter::isMoving()) {
                                        currState = 0;
                                    }
                                    break;
                            }
                            break;
                        // invalid
                        default:
                            currState = 0;
                            break;
                    }
                    break;
                // M-codes
                case 'M':
                    switch (currCommand->iNum) {
                        // M0 unconditional stop
                        case 0:
                            input::sendMessage(F("M0 Shutting down."));
                            shutdownMachine();
                            currState = 0;
                            break;
                        //M03/M04 spindle on
                        case 3:
                        case 4: {
                            Field* fS = findField('S');
                            
                            if (fS != nullptr) {
                                laser::setLaserLevel(fS->iNum);
                            }
                        
                            laser::laserPowerOn();
                            currState = 0;
                            break;
                        }
                        //M05 spindle off
                        case 5:
                            laser::laserPowerOff();
                            currState = 0;
                            break;
                        //enable all motors
                        case 17:
                            plotter::getXStepper()->enable();
                            plotter::getYStepper()->enable();
                            currState = 0;
                            break;
                        //disable all motors
                        case 18:
                            plotter::getXStepper()->disable();
                            plotter::getYStepper()->disable();
                            currState = 0;
                            break;
                        // get laser power
                        case 105:
                            input::sendMessage(F("M105 "));
                            if (laser::isLaserOn()) {
                                input::sendInt(laser::getLaserLevel());
                            } else {
                                input::sendInt(-1);
                            }
                            input::sendChar('\n');
                            currState = 0;
                            break;
                        //M114 get position
                        case 114: {
                            input::sendMessage(F("M114 X:"));
                            dist_mm_d x = umToMm_dec(plotter::getXLocation());
                            input::sendInt(x.mm);
                            input::sendChar('.');
                            input::sendInt(x.dec);
                            input::sendMessage(F(" Y:"));
                            dist_mm_d y = umToMm_dec(plotter::getYLocation());
                            input::sendInt(y.mm);
                            input::sendChar('.');
                            input::sendInt(y.dec);
                            input::sendChar('\n');
                            currState = 0;
                            break;
                        }
                        // get firmware version
                        case 115: {
                            input::sendMessage(F("M115 "));
                            input::sendMessage(F(FIRMWARE_VERSION));
                            input::sendChar('\n');
                            currState = 0;
                            break;
                        }
                        //M145 print internal state
                        case 145:
                            input::sendMessage(F("M145\n"));
                            input::printDebug();
                            parser::printDebug();
                            plotter::printDebug();
                            safety::printDebug();
                            //scheduler::printDebug();
                            input::sendMessage(F("EOL\n"));
                            currState = 0;
                            break;
                        //M400 flush buffer
                        case 400:
                            // by the time we get here, the buffer is empty.
                            // send OK to finally acknowledge the command and allow sending
                            input::m114Finished();
                            
                            currState = 0;
                            break;
                        // invalid
                        default:
                            currState = 0;
                            break;
                    }
                    break;
                // invalid codes
                default:
                    currState = 0;
                    break;
            }
        }
    }
}
