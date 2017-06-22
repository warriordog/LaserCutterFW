#include "LaserFW.h"
#include "Stepper.h"
#include "Plotter.h"

void laserSetup() {
    
}

void laserLoop() {
    
}


void shutdownMachine() {
    plotter::cancelMovement();
    laser::laserPowerOff();
    parser::clearBuffer();
}

