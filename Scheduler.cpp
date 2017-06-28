#include "Scheduler.h"
#include "Plotter.h"
#include "Parser.h"
#include "Input.h"

namespace scheduler {

    void tick() {
        // update movement
        if (plotter::isMoving()) {
            plotter::updateMovement();
        // start next task
        } else if (parser::hasWork()) {
            parser::startNextCode();
        }
        
        // refill parser queue
        if (parser::hasQueueSpace()) {
            if (input::lineReady()) {
                parser::addLine(input::takeLine());
            }
        }
        
        // read from serial
        if (!input::lineReady()) {
            input::poll();
        }
    }
}
