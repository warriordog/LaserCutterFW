#ifndef STEPPER_H
#define STEPPER_H

#include "BasicStepperDriver.h"
#include "Units.h"

namespace stepper {
    class Stepper {
        private:
            int stepPin;
            int dirPin;
            int enablePin;
            
            BasicStepperDriver driver;
            step_step lastCommandSteps;
            
            bool isEnabled = false;
        public:
            Stepper(int stepPin, int directionPin, int enablePin);
        
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
