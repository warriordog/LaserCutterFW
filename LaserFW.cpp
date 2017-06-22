#include "LaserFW.h"
#include "Stepper.h"
#include "Plotter.h"
#include "Laser.h"
#include "Parser.h"
#include "Scheduler.h"
#include "Safety.h"
#include "Input.h"
#include "Lights.h"

bool machineRunning = true;

void laserSetup() {
    lights::setup();
    input::setup();
    plotter::setup();
    laser::setup();
    safety::setup(); // start up last because of WDT
}

void laserLoop() {
    while (machineRunning) {
        safety::tick(); // rest of loop has 4 seconds to complete or chip resets
        scheduler::tick();
    }
}


void shutdownMachine() {
    machineRunning = false;
    safety::shutdown(); //shutdown first because of WDT
    plotter::cancelMovement();
    laser::laserPowerOff();
    parser::clearBuffer();
    lights::shutdown();
}

