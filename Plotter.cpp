#include "Plotter.h"
#include "Config.h"
#include <Arduino.h>
#include "Constants.h"

// TODO rewrite completely using um units, no floats, and RPM control

namespace plotter {
    /*
        Settings
    */
    
    //movement speed of axis (mm / min)
    int xSpeed = 0;
    int ySpeed = 0;

    /*
        Current state
    */

    //current location of axis (um)
    long xLocation = 0;
    long yLocation = 0;
    
    //if an axis is currently moving
    bool xMoving = false;
    bool yMoving = false;
    
    /*
        Movement
    */
    
    //target of current movement (um)
    long xTarget = 0;
    long yTarget = 0;
    
    //location of axis at start of movement (um)
    long xStart = 0;
    long yStart = 0;
    
    //time that target location was reached
    unsigned long timeArrived = 0;
    
    //time that the current movement started
    unsigned long movementStartTime = 0;
       
    /*
        Motors
    */
    
    stepper::Stepper* xStepper = nullptr;
    stepper::Stepper* yStepper = nullptr;
    
    /*
        Runtime-generated constants
    */
    const unsigned long X_UM_PER_STEP = (unsigned long)(1.0 / ((float)X_STEPS_PER_MM / 1000.0));
    const unsigned long Y_UM_PER_STEP = (unsigned long)(1.0 / ((float)Y_STEPS_PER_MM / 1000.0));
    
    /*
        -----------------------------
        Functions
        -----------------------------
    */
    
    void setup() {
        xStepper = new stepper::Stepper(PIN_X_STEP, PIN_X_DIR, PIN_X_EN);
        yStepper = new stepper::Stepper(PIN_Y_STEP, PIN_Y_DIR, PIN_Y_EN);
    }
    
    void setTarget(int x, int y) {setTargetX(x); setTargetY(y);}
    
    void setXSpeed(int vel) {xSpeed = vel;}
    void setYSpeed(int vel) {ySpeed = vel;}
    
    bool isMovingX() {return xMoving;}
    bool isMovingY() {return yMoving;}
    bool isMoving() {return isMovingX() || isMovingY();}
    
    unsigned long getTimeArrived() {return timeArrived;}

    void setTargetX(int target) {
        if (target < X_AXIS_MIN) {
            target = X_AXIS_MIN;
        }
        if (target > X_AXIS_MAX) {
            target = X_AXIS_MAX;
        }
        if (target != xLocation) {
            xTarget = target;
            xMoving = true;
            movementStartTime = millis();
        }
    }
    
    void setTargetY(int target) {
        if (target < Y_AXIS_MIN) {
            target = Y_AXIS_MIN;
        }
        if (target > Y_AXIS_MAX) {
            target = Y_AXIS_MAX;
        }
        if (target != yLocation) {
            yTarget = target;
            yMoving = true;
            movementStartTime = millis();
        }
    }

    unsigned long calcCurMMPerMin(unsigned long curTimeMS, unsigned long curDistUM) {
        float curDistMM = (float)curDistUM / 1000.0;
        float curMMperMS = curDistMM / (float)curTimeMS;
        unsigned long curMMperSec = (unsigned int)(curMMperMS * 1000.0);
        unsigned long curMMperMin = curMMperSec * 60;
        return curMMperMin;
    }

    /*
      Progresses the current movement.
      
      Decides whether to tick a motor during this cycle by calculating the average feed rate of the movement so far.
      
      Possible optimization: change units of xVelocity and yVelocity to be a float MM/MS
      Possible alternative: always tick but adjust RPM of motor
    */
    void updateMovement() {
        //skip this whole giant fucntion if we are not moving
        if (isMoving()) {
            // calculate duration of this movement (so far)
            unsigned long curTimeMS = millis() - movementStartTime;
            if (curTimeMS == 0) {
                curTimeMS = 1;
            }
            
            //update x axis movement
            if (isMovingX()) {
                unsigned long curDistUM = abs(xLocation - xStart);
                unsigned long curMMperMin = calcCurMMPerMin(curTimeMS, curDistUM);
                
                if (curMMperMin <= xSpeed) {
                    long distanceUM = X_UM_PER_STEP;
                    
                    //moving forward
                    if (xTarget > xLocation) {
                        xStepper->moveByStep(1);
                        xLocation += distanceUM;
                        
                        //if we arrive then end movement
                        if (xLocation >= xTarget) {
                            xMoving = false;
                        }
                    //moving backwards
                    } else {
                        xStepper->moveByStep(-1);
                        xLocation -= distanceUM;
                        
                        //if we arrive then end movement
                        if (xLocation <= xTarget) {
                            xMoving = false;
                        }
                    }
                }
            }
            
            //update y axis movement
            if (isMovingY()) {
                unsigned long curDistUM = abs(xLocation - xStart);
                unsigned long curMMperMin = calcCurMMPerMin(curTimeMS, curDistUM);
                
                if (curMMperMin <= ySpeed) {
                    long distanceUM = Y_UM_PER_STEP;
                    
                    //moving forward
                    if (yTarget > yLocation) {
                        yStepper->moveByStep(1);
                        yLocation += distanceUM;
                        
                        //if we arrive then end movement
                        if (yLocation >= yTarget) {
                            yMoving = false;
                        }
                    //moving backwards
                    } else {
                        yStepper->moveByStep(-1);
                        yLocation -= distanceUM;
                        
                        //if we arrive then end movement
                        if (yLocation <= yTarget) {
                            yMoving = false;
                        }
                    }
                }
            }
        
            // if we aren't moving now, then both axis finished
            if (!isMoving()) {
                timeArrived = millis();
            }
        }
    }
    
    void cancelMovement() {
        xMoving = false;
        yMoving = false;
        
        xTarget = xLocation;
        yTarget = yLocation;
        
        timeArrived = millis();
    }
    
    
    long getXLocation() {
        return xLocation;
    }
    
    long getYLocation() {
        return yLocation;
    }
}
