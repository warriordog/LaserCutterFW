#ifndef PLOTTER_H
#define PLOTTER_H
#include "Stepper.h"

namespace plotter {
    bool hasXMotor();
    bool hasYMotor();
    
    void setXMotor(stepper::Stepper* stepper);
    void setYMotor(stepper::Stepper* stepper);
    
    void setTargetX(int target);
    void setTargetY(int target);
    void setTarget(int x, int y);
    
    void setXSpeed(int vel);
    void setYSpeed(int vel);
    
    bool isMovingX();
    bool isMovingY();
    bool isMoving();
    
    unsigned long getTimeArrived();
    
    void updateMovement();
}

#endif //PLOTTER_H
