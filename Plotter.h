#ifndef PLOTTER_H
#define PLOTTER_H
#include "Stepper.h"
#include "Units.h"

namespace plotter {
    void setup();
    
    void setTargetX(dist_um target);
    void setTargetY(dist_um target);
    void setTarget(dist_um x, dist_um y);
    
    void setXSpeed(mm_per_min vel);
    void setYSpeed(mm_per_min vel);
    
    bool isMoving();
    
    dist_um getXLocation();
    dist_um getYLocation();
    
    time_ms getTimeArrived();
    
    void updateMovement();
    
    // cancel in progress movement
    void cancelMovement();
}

#endif //PLOTTER_H
