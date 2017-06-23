#ifndef STEPPER_H
#define STEPPER_H

#include "BasicStepperDriver.h"
#include "Units.h"

namespace stepper {
    class Stepper {
        private:
            GPIO_pin_t stepPin;
            GPIO_pin_t dirPin;
            GPIO_pin_t enablePin;
            
            BasicStepperDriver driver;
            step_step lastCommandSteps;
            
            bool isEnabled = true;
        public:
            Stepper(GPIO_pin_t stepPin, GPIO_pin_t directionPin, GPIO_pin_t enablePin);
        
            void enable() {
                isEnabled = true;
                driver.enable();
            }
            void disable() {
                isEnabled = false;
                driver.disable();
            }
            bool checkEnabled() {return isEnabled;}
            
            void moveByDeg(step_deg degrees);
            void moveByStep(step_step steps);
            
            void setRPM(int rpm);
            
            void tickDriver();
            bool isMoving();
            step_step getCurrentProgress();
            
            void printDebug();
    };
}

#endif //STEPPER_H
