#include "Plotter.h"
#include "Config.h"
#include <Arduino.h>
#include "Constants.h"
#include "Units.h"

namespace plotter {
    enum motor_state {
        // not moving
        IDLE,
        
        // movement requested, but command not send
        WAITING,
        
        // one or more motors is moving
        MOVING
    };

    /*
        Settings
    */
    
    //movement speed of axis (mm / min)
    mm_per_min xSpeed = 0;
    mm_per_min ySpeed = 0;

    /*
        Current state
    */

    //current location of axis (um)
    dist_um xLocation = 0;
    dist_um yLocation = 0;
    
    //if an axis is currently moving
    motor_state motorState = IDLE;
    
    /*
        Movement
    */
    
    //target of current movement (um)
    dist_um xTarget = 0;
    dist_um yTarget = 0;
    
    //location of axis at start of movement (um)
    dist_um xStart = 0;
    dist_um yStart = 0;
    
    //time that target location was reached
    time_ms timeArrived = 0;
    
    //time that the current movement started
    time_ms movementStartTime = 0;
       
    /*
        Motors
    */
    
    stepper::Stepper* xStepper = nullptr;
    stepper::Stepper* yStepper = nullptr;
    
    /*
        -----------------------------
        Functions
        -----------------------------
    */
    
    void setup() {
        xStepper = new stepper::Stepper(PIN_X_STEP, PIN_X_DIR, PIN_X_EN);
        yStepper = new stepper::Stepper(PIN_Y_STEP, PIN_Y_DIR, PIN_Y_EN);
    }
    
    void setTarget(dist_um x, dist_um y) {setTargetX(x); setTargetY(y);}
    
    void setXSpeed(mm_per_min vel) {xSpeed = vel;}
    void setYSpeed(mm_per_min vel) {ySpeed = vel;}
    
    bool isMoving() {return motorState == MOVING;}
    
    time_ms getTimeArrived() {return timeArrived;}

    void setTargetX(dist_um target) {
        if (target < X_AXIS_MIN) {
            target = X_AXIS_MIN;
        }
        if (target > X_AXIS_MAX) {
            target = X_AXIS_MAX;
        }
        if (target != xLocation) {
            xTarget = target;
            motorState = WAITING;
            movementStartTime = millis();
        }
    }
    
    void setTargetY(dist_um target) {
        if (target < Y_AXIS_MIN) {
            target = Y_AXIS_MIN;
        }
        if (target > Y_AXIS_MAX) {
            target = Y_AXIS_MAX;
        }
        if (target != yLocation) {
            yTarget = target;
            motorState = WAITING;
            movementStartTime = millis();
        }
    }

    /*
      Plans and starts a motor movement
    */
    void startMovement() {
        // in case neither motor starts moving for some reason.
        motorState = IDLE;
    
        dist_um xDist = abs(xTarget - xLocation);
        dist_um yDist = abs(yTarget - yLocation);
        
        if (xDist != 0) {
            step_step steps = calcStepsForUm(xDist);
            if (xTarget < xLocation) {
                steps *= -1;
            }
            
            step_rpm rpm = calcRPM(xDist, xSpeed);
            if (rpm > MAX_RPM) {
                rpm = MAX_RPM;
            }
            xStepper->setRPM(rpm);
            
            xStepper->moveByStep(steps);
            motorState = MOVING;
        }
        
        if (yDist != 0) {
            step_step steps = calcStepsForUm(yDist);
            if (yTarget < yLocation) {
                steps *= -1;
            }
            
            step_rpm rpm = calcRPM(xDist, ySpeed);
            if (rpm > MAX_RPM) {
                rpm = MAX_RPM;
            }
            yStepper->setRPM(rpm);
            
            yStepper->moveByStep(steps);
            motorState = MOVING;
        }
    }

    /*
      Progresses the current movement.
    */
    void updateMovement() {
        switch (motorState) {
            case IDLE:
                break;
            case WAITING:
                startMovement();
                motorState = MOVING;
                break;
            case MOVING: {
                bool isMoving = false;
                if (xStepper->isMoving()) {
                    xStepper->tickDriver();
                    isMoving = true;
                }
                if (yStepper->isMoving()) {
                    yStepper->tickDriver();
                    isMoving = true;
                }
                
                // if both motors are finished, then stop ticking
                if (!isMoving) {
                    motorState = IDLE;
                }
                break;
            }
            default:
                //should not happen
                motorState = IDLE;
                break;
        }
    }
    
    void cancelMovement() {
        motorState = IDLE;
        
        xTarget = xLocation;
        yTarget = yLocation;
        
        timeArrived = millis();
    }
    
    
    dist_um getXLocation() {
        return xLocation;
    }
    
    dist_um getYLocation() {
        return yLocation;
    }
}
