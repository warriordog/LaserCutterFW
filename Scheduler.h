#ifndef SCHEDULER_H
#define SCHEDULER_H

namespace scheduler {
    enum MoveState {
        // machine is doing nothing
        IDLE,
        
        // machine is only moving
        MOVING,
        
        // machine is executing a gcode
        EXECUTING
    };
    
    void tick();
}

#endif //SCHEDULER_H
