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
            
            int currPosition;
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
            
            void moveToDeg(int degrees);
            void moveByDeg(int degrees);
            void moveByStep(int steps);
    };
}

#endif //STEPPER_H
