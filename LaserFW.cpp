#include "LaserFW.h"
#include "Stepper.h"
#include "Plotter.h"
#include "Laser.h"
#include "Parser.h"
#include "Scheduler.h"
#include "Safety.h"
#include "Input.h"

bool machineRunning = true;

void laserSetup() {
    safety::setup();
    input::setup();
    plotter::setup();
    laser::setup();
}

void laserLoop() {
    while (machineRunning) {
        safety::tick(); // rest of loop has 4 seconds to complete or chip resets
        scheduler::tick();
    }
}


void shutdownMachine() {
    machineRunning = false;
    safety::shutdown();
    plotter::cancelMovement();
    laser::laserPowerOff();
    parser::clearBuffer();
}

