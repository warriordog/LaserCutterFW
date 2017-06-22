#include "Safety.h"
#include <avr/wdt.h>
#include "Config.h"
#include "Plotter.h"
#include <Arduino.h>
#include "Laser.h"

namespace safety {
    // if true, then the laser is shut off until it moves
    bool laserSafetyEngaged = false;
    int laserLevel = 0;
    long lastMoveX = 0;
    long lastMoveY = 0;
    unsigned long lastMoveTime = 0;
    
    // Forward declarations
    void checkLaserDwell();
    
    void setup() {
        wdt_enable(WDTO_4S);
    }
    
    void tick() {
        checkLaserDwell();
        wdt_reset();
    }
    
    void shutdown() {
        wdt_disable();
    }
    
    void checkLaserDwell() {
        // if laser safety is engaged then check if laser has moved
        if (laserSafetyEngaged) {
            // laser has moved, turn back on
            if (lastMoveX != plotter::getXLocation() || lastMoveY != plotter::getYLocation()) {
                laserSafetyEngaged = false;
                laser::laserPowerOn();
                laser::setLaserLevel(laserLevel);
            }
        // if safety is off then do normal checks
        } else {
            // if laser is on, check timer
            if (laser::isLaserOn()) {
                // laser time limit reached
                if (millis() - lastMoveTime > LASER_DWELL_LIMIT) {
                    laserSafetyEngaged = true;
                    laserLevel = laser::getLaserLevel(); //store power level since powerOff resets it
                    laser::laserPowerOff();
                }
            // if laser is off and safety not engaged, then reset timer and location
            } else {
                lastMoveTime = millis();
            }
        }
        
        // if laser has moved, then we already checked above.  If not, then this does nothing.
        lastMoveX = plotter::getXLocation();
        lastMoveY = plotter::getYLocation();
    }
}
