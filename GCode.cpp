#include "GCode.h"
#include "Plotter.h"
#include "Laser.h"
#include "Parser.h"
#include "LaserFW.h"
#include "Input.h"
#include "Lights.h"

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
    /*
        Serial.print(F("Adding: '"));
        Serial.print(field->letter);
        Serial.print(field->iNum);
        Serial.println('\'');
      
      */
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
            
            /*
            input::sendMessage(F("Executing: '"));
            input::sendChar(currCommand->letter);
            input::sendInt(currCommand->iNum);
            
            if (currArgs != nullptr) {
                for (FieldNode* node = currArgs->getHead(); node != nullptr; node = node->next) {
                    input::sendMessage(F(" "));
                    input::sendChar(node->field->letter);
                    input::sendInt(node->field->iNum);
                }
            }
            
            input::sendMessage(F("'\n"));
            */
        }
    }
    
    bool isWorking() {
        return currState != 0;
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
                        //M114 get position
                        case 114:
                            input::sendMessage(F("X:"));
                            input::sendInt(plotter::getXLocation());
                            input::sendMessage(F(" Y:"));
                            input::sendInt(plotter::getYLocation());
                            input::sendMessage(F("\n"));
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
