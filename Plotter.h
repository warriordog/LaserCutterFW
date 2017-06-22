#ifndef PLOTTER_H
#define PLOTTER_H
#include "Stepper.h"

namespace plotter {
    void setup();
    
    void setTargetX(int target);
    void setTargetY(int target);
    void setTarget(int x, int y);
    
    void setXSpeed(int vel);
    void setYSpeed(int vel);
    
    bool isMovingX();
    bool isMovingY();
    bool isMoving();
    
    long getXLocation();
    long getYLocation();
    
    unsigned long getTimeArrived();
    
    void updateMovement();
    
    // cancel in progress movement
    void cancelMovement();
}

#endif //PLOTTER_H
