#include "Safety.h"
#include <avr/wdt.h>
#include "Config.h"
#include "Plotter.h"
#include <Arduino.h>
#include "Laser.h"
#include "Input.h"
#include "Units.h"

namespace safety {
    // if true, then the laser is shut off until it moves
    bool laserSafetyEngaged = false;
    int laserLevel = 0;
    long lastMoveX = 0;
    long lastMoveY = 0;
    time_ms lastMoveTime = 0;
    
    time_us lastTickTime = 0;
    time_us totTickDuration = 0;
    unsigned long numTicks = 0;
    
    // if laser safety is disabled
    bool laserSafetyDisabled = false;
    // time when laser safety was disabled
    time_ms laserSafetyDisabledStart = 0;
    time_ms laserSafetyDisabledTime = 0;
    
    // Forward declarations
    void checkLaserDwell();
    void updateTickTime();
    void checkLaserDisabledTimeout();
    
    void setup() {
        wdt_enable(WDTO_4S);
    }
    
    void tick() {
        updateTickTime();
        checkLaserDisabledTimeout();
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
                if (!laserSafetyDisabled && millis() - lastMoveTime > LASER_DWELL_LIMIT) {
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
    
    void updateTickTime() {
        time_us currTime = micros();
        time_us thisDur = currTime - lastTickTime;
        
        // record and increment tick duration
        time_us lastTotDur = totTickDuration;
        totTickDuration += thisDur;
        
        // record and increment number of ticks
        time_us lastNumTicks = numTicks;
        numTicks++;
        
        // check for overflow and reset
        if (totTickDuration < lastTotDur || numTicks < lastNumTicks) {
            totTickDuration = (lastTotDur / lastNumTicks) + thisDur;
            numTicks = 2;
        }
        
        lastTickTime = currTime;    
    }
    
    time_us getAvgTickDuration() {
        if (numTicks == 0) {
            return 0;
        }
        return totTickDuration / numTicks;
    }
    
    void disableLaserSafety(time_sec time) {
        laserSafetyDisabled = true;
        laserSafetyDisabledStart = millis() + (time * 1000l);
        laserSafetyDisabledTime = time * 1000l;
    }
    
    void enableLaserSafety() {
        laserSafetyDisabled = false;
        laserSafetyDisabledStart = 0;
        laserSafetyDisabledTime = 0;
    }
    
    void checkLaserDisabledTimeout() {
        if (laserSafetyDisabled) {
            if (millis() - laserSafetyDisabledStart > laserSafetyDisabledTime) {
                enableLaserSafety();
            }
        }
    }
    
    void printDebug() {
        input::sendMessage(F("safety::laserSafetyEngaged="));
        input::sendBool(laserSafetyEngaged);
        input::sendMessage(F("\nsafety::laserLevel="));
        input::sendInt(laserLevel);
        input::sendMessage(F("\nsafety::lastMoveX="));
        input::sendLong(lastMoveX);
        input::sendMessage(F("\nsafety::lastMoveY="));
        input::sendLong(lastMoveY);
        input::sendMessage(F("\nsafety::lastMoveTime="));
        input::sendULong(lastMoveTime);
        input::sendMessage(F("\nsafety::lastTickTime="));
        input::sendULong(lastTickTime);
        input::sendMessage(F("\nsafety::totTickDuration="));
        input::sendULong(totTickDuration);
        input::sendMessage(F("\nsafety::numTicks="));
        input::sendULong(numTicks);
        input::sendMessage(F("\nsafety::getAvgTickDuration()="));
        input::sendULong(getAvgTickDuration());
        input::sendMessage(F("\nsafety::laserSafetyDisabled="));
        input::sendBool(laserSafetyDisabled);
        input::sendMessage(F("\nsafety::laserSafetyDisableStart="));
        input::sendULong(laserSafetyDisabledStart);
        input::sendMessage(F("\nsafety::laserSafetyDisableTime="));
        input::sendULong(laserSafetyDisabledTime);
        input::sendChar('\n');
    }
}
