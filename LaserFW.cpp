#include "LaserFW.h"
#include "Stepper.h"
#include "Plotter.h"
#include "Laser.h"
#include "Parser.h"

void laserSetup() {
    
}

void laserLoop() {
    
}


void shutdownMachine() {
    plotter::cancelMovement();
    laser::laserPowerOff();
    parser::clearBuffer();
}

