#ifndef STEPPER_H
#define STEPPER_H

#include "BasicStepperDriver.h"

namespace stepper {
    class Stepper {
        private:
            GPIO_pin_t stepPin;
            GPIO_pin_t dirPin;
            GPIO_pin_t enablePin;
            
            BasicStepperDriver driver;
            
            bool isEnabled;
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
            
            void moveByDeg(int degrees);
            void moveByStep(int steps);
            
            void setRPM(int rpm);
            
            void tickDriver();
            bool isMoving();
            
            void printDebug();
    };
}

#endif //STEPPER_H
