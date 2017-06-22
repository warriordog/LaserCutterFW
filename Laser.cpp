#include "Laser.h"
#include "Constants.h"
#include <Arduino.h>
#include <arduino2.h>

namespace laser {
    bool laserPower = false;
    int laserLevel = 0;
    
    void setup() {
        pinMode(PIN_LASER_TTL, OUTPUT);        
        pinMode2(PIN_LASER_POWER, OUTPUT);
        
        laserPowerOff();
    }

    void laserPowerOff() {
        //active high?
        digitalWrite2(PIN_LASER_POWER, LOW);
        laserPower = false;
        setLaserLevel(0);
    }
    
    void laserPowerOn() {
        // active high?
        digitalWrite2(PIN_LASER_POWER, HIGH);
        laserPower = true;
    }
    
    void setLaserLevel(int level) {
        if (level < 0) {
            level = 0;
        }
        if (level > 255) {
            level = 255;
        }
        analogWrite(PIN_LASER_TTL, 0);
        laserLevel = level;
    }
    
    
    bool isLaserOn() {
        return laserPower;
    }
    
    int getLaserLevel() {
        return laserLevel;
    }
}
