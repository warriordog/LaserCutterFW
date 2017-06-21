#include "Stepper.h"
#include "BasicStepperDriver.h"

namespace stepper {
    Stepper::Stepper (GPIO_pin_t stepPin, GPIO_pin_t directionPin, GPIO_pin_t enablePin)
        //create stepper with 1.8 degrees / step
        : driver(200, directionPin, stepPin, enablePin) {
        driver.setRPM(120); //120 RPM is safe
        driver.setMicrostep(16); //jumpers set to 128, but controllers only support 16
    }

    void Stepper::moveToDeg (int degrees) {
        if (isEnabled) {
            //limit to good range
            //degrees = degrees % 360;
            
            int diff = degrees - currPosition;
            moveByDeg(diff);
        }
    }

    void Stepper::moveByDeg (int degrees) {
        if (isEnabled) {
            if (degrees != 0) {
                driver.rotate(degrees);
            }
        }
    }

    void Stepper::moveByStep (int steps) {
        if (isEnabled) {
            if (steps != 0) {
                driver.move(steps);
            }
        }
    }
}
