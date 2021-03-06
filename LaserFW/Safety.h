#ifndef SAFETY_H
#define SAFTEY_H
#include "Units.h"

namespace safety {
    void setup();
    void tick();
    void shutdown();
    
    time_ms getAvgTickDuration();
    
    void disableLaserSafety(time_sec time);
    void enableLaserSafety();
    
    bool isLaserSafetyEngaged();
    
    void printDebug();
}

#endif //SAFETY_H
