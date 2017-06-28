#include "Stepper.h"
#include "BasicStepperDriver.h"
#include "Config.h"
#include "Constants.h"
#include "Input.h"
#include "Units.h"

namespace stepper {
    Stepper::Stepper(int stepPin, int directionPin, int enablePin)
        //create stepper with 1.8 degrees / step
        : driver(STEPS_PER_ROTATION, directionPin, stepPin, enablePin) {
        driver.setRPM(DEFAULT_RPM); //120 RPM is safe
        driver.setMicrostep(MOTOR_MICROSTEPS); //jumpers set to 128, but controllers only support 16
    }

    void Stepper::moveByDeg(step_deg degrees) {
        if (isEnabled) {
            if (degrees != 0) {
                driver.rotate((int)degrees);
            }
        }
    }

    void Stepper::moveByStep(step_step steps) {
        if (isEnabled) {
            if (steps != 0) {
                driver.move(steps);
            }
        }
    }
    
    step_step Stepper::getCurrentProgress() {
        return lastCommandSteps - driver.getStepsRemaining();
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
    
    void Stepper::printDebug() {
        input::sendMessage(F("stepper::Stepper.stepPin="));
        input::sendInt(stepPin);
        input::sendMessage(F("\nstepper::Stepper.dirPin="));
        input::sendInt(dirPin);
        input::sendMessage(F("\nstepper::Stepper.enablePin="));
        input::sendInt(enablePin);
        input::sendMessage(F("\nstepper::Stepper.isEnabled="));
        input::sendBool(isEnabled);
        input::sendChar('\n');
    }
}
