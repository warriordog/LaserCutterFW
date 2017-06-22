#include "Lights.h"
#include "Constants.h"
#include <arduino2.h>
#include <Arduino.h>
#include "Config.h"

namespace lights {
    bool actOn = false;
    unsigned long actStart = 0;
    
    bool pwrBlinking = false;
    unsigned long pwrBlinkStart = 0;
    unsigned long pwrBlinkDuration = 0;
    unsigned long pwrBlinkInterval = 0;

    void updateLife() {
        // moves from 0 to 510 across ~2 seconds 
        int lightTime = ((millis() / 4) % 510);
        
        //reverse the second half of the spectrum
        if (lightTime > 255) {
            lightTime = 255 - (510 - lightTime);
        }
        
        // will be a value from 0 - 255 by now
        analogWrite(PIN_LED_LIFE, lightTime);
    }

    void updateActivity() {
        if (actOn) {
            if (millis() - actStart > LED_ACT_DURATION) {
                digitalWrite2(PIN_LED_ACT, LOW);
                actOn = false;
            } else {
                digitalWrite2(PIN_LED_ACT, HIGH);
            }
        }
    }
    
    void updateBlink() {
        if (pwrBlinking) {
            if (millis() - pwrBlinkStart > pwrBlinkDuration) {
                digitalWrite2(PIN_LED_POWER, LOW);
                pwrBlinking = false;
            } else {
                // turn off or by by interval
                if (((millis() - pwrBlinkStart) / pwrBlinkInterval) % 2 == 0) {
                    digitalWrite2(PIN_LED_POWER, HIGH);
                } else {
                    digitalWrite2(PIN_LED_POWER, LOW);
                }
            }
        }
    }

    void setup() {
        pinMode2(PIN_LED_POWER, OUTPUT);
        pinMode2(PIN_LED_ACT, OUTPUT);
        pinMode(PIN_LED_LIFE, OUTPUT);
        
        digitalWrite2(PIN_LED_POWER, HIGH);
        digitalWrite2(PIN_LED_ACT, LOW);
        updateLife();
    }
    
    void tick() {
        updateLife();
        updateActivity();
        updateBlink();
    }
    
    void shutdown() {
        digitalWrite2(PIN_LED_POWER, LOW);
        digitalWrite2(PIN_LED_ACT, LOW);
        analogWrite(PIN_LED_LIFE, 0);
    }
    
    void blinkActivity() {
        actOn = true;
        actStart = millis();
    }
    
    void blinkSafety() {
        pwrBlinking = true;
        pwrBlinkStart = millis();
        pwrBlinkDuration = LED_SAFETY_DURATION;
        pwrBlinkInterval = LED_SAFETY_INTERVAL;
    }
    
    void blinkError() {
        pwrBlinking = true;
        pwrBlinkStart = millis();
        pwrBlinkDuration = LED_ERROR_DURATION;
        pwrBlinkInterval = LED_ERROR_INTERVAL;
    }
}
