#include "Stepper.h"
#include "BasicStepperDriver.h"
#include "Config.h"
#include "Constants.h"

namespace stepper {
    Stepper::Stepper (GPIO_pin_t stepPin, GPIO_pin_t directionPin, GPIO_pin_t enablePin)
        //create stepper with 1.8 degrees / step
        : driver(STEPS_PER_ROTATION, directionPin, stepPin, enablePin) {
        driver.setRPM(DEFAULT_RPM); //120 RPM is safe
        driver.setMicrostep(MOTOR_MICROSTEPS); //jumpers set to 128, but controllers only support 16
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
    
    
    void Stepper::setRPM(int rpm) {
        if (rpm < 0) {
            rpm = 0;
        }
        if (rpm > MAX_RPM) {
            rpm = MAX_RPM;
        }
        driver.setRPM(rpm);
    }
    
    void Stepper::tickDriver() {
        driver.tickMovement();
    }
    
    bool Stepper::isMoving() {
        return driver.isMoving();
    }
}
