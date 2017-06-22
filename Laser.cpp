#include "Laser.h"
#include "Constants.h"
#include <Arduino.h>
#include <arduino2.h>

namespace laser {
    void setup() {
        pinMode(PIN_LASER_TTL, OUTPUT);
        setLaserLevel(0);
        
        pinMode2(PIN_LASER_POWER, OUTPUT);
        digitalWrite2(PIN_LASER_POWER, LOW);
    }

    void laserPowerOff() {
        //active high?
        digitalWrite2(PIN_LASER_POWER, LOW);
        setLaserLevel(0);
    }
    
    void laserPowerOn() {
        digitalWrite2(PIN_LASER_POWER, HIGH);
    }
    
    void setLaserLevel(int level) {
        analogWrite(PIN_LASER_TTL, 0);
    }
}
