#include "Plotter.h"
#include "Config.h"
#include <Arduino.h>

namespace plotter {
    const unsigned long X_UM_PER_STEP = (unsigned long)(1.0 / ((float)X_STEPS_PER_MM / 1000.0));

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

    unsigned long calcCurMMPerMin(unsigned long curDistUM) {
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
            if (isMovingX() && hasXMotor()) {
                unsigned long curDistUM = abs(xLocation - xStart);
                unsigned long curMMperMin = calcCurMMPerMin(curDistUM);
                
                if (curMMperMin <= xVelocity) {
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
            if (isMovingY() && hasYMotor()) {
                unsigned long curDistUM = abs(xLocation - xStart);
                unsigned long curMMperMin = calcCurMMPerMin(curDistUM);
                
                if (curMMperMin <= yVelocity) {
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
}
