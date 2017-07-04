#ifndef PLOTTER_H
#define PLOTTER_H
#include "Stepper.h"
#include "Units.h"

namespace plotter {
    void setup();
    
    void setTargetX(dist_um target);
    void setTargetY(dist_um target);
    void setTarget(dist_um x, dist_um y);
    void setTargetX(dist_mm_d x);
    void setTargetY(dist_mm_d y);
    void setTarget(dist_mm_d x, dist_mm_d y);
    
    void setXSpeed(mm_per_min vel);
    void setYSpeed(mm_per_min vel);
    mm_per_min getXSpeed();
    
    bool isMoving();
    
    dist_um getXLocation();
    dist_um getYLocation();
    
    void updateMovement();
    
    // cancel in progress movement
    void cancelMovement();
    
    stepper::Stepper* getXStepper();
    stepper::Stepper* getYStepper();
    
    void printDebug();
}

#endif //PLOTTER_H
