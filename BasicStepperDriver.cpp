/*
 * Generic Stepper Motor Driver Driver
 * Indexer mode only.
 *
 * Copyright (C)2015 Laurentiu Badea
 *
 * Modified to use dio2 library for fast IO.  Normal arduino digitalWrite/etc are replaced with digitalWrite2/etc2.
 * Modified to step asynchronously.
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "BasicStepperDriver.h"

/*
 * Basic connection: only DIR, STEP are connected.
 * Microstepping controls should be hardwired.
 */
BasicStepperDriver::BasicStepperDriver(int steps, GPIO_pin_t dir_pin, GPIO_pin_t step_pin)
:motor_steps(steps), dir_pin(dir_pin), step_pin(step_pin)
{
    init();
}

BasicStepperDriver::BasicStepperDriver(int steps, GPIO_pin_t dir_pin, GPIO_pin_t step_pin, GPIO_pin_t enable_pin)
:motor_steps(steps), dir_pin(dir_pin), step_pin(step_pin), enable_pin(enable_pin)
{
    init();
}

void BasicStepperDriver::init(void){
    pinMode2f(dir_pin, OUTPUT);
    digitalWrite2f(dir_pin, HIGH);

    pinMode2f(step_pin, OUTPUT);
    digitalWrite2f(step_pin, LOW);

    if IS_CONNECTED(enable_pin){
        pinMode2f(enable_pin, OUTPUT);
        digitalWrite2f(enable_pin, HIGH); // disable
    }

    setMicrostep(1);
    setRPM(60); // 60 rpm is a reasonable default

    enable();
}


void BasicStepperDriver::calcStepPulse(void){
    step_pulse = STEP_PULSE(rpm, motor_steps, microsteps);
}

/*
 * Set target motor RPM (1-200 is a reasonable range)
 */
void BasicStepperDriver::setRPM(unsigned rpm){
    this->rpm = rpm;
    calcStepPulse();
}

/*
 * Set stepping mode (1:microsteps)
 * Allowed ranges for BasicStepperDriver are 1:1 to 1:128
 */
unsigned BasicStepperDriver::setMicrostep(unsigned microsteps){
    for (unsigned ms=1; ms <= this->getMaxMicrostep(); ms<<=1){
        if (microsteps == ms){
            this->microsteps = microsteps;
            break;
        }
    }
    calcStepPulse();
    return this->microsteps;
}

/*
 * DIR: forward HIGH, reverse LOW
 */
void BasicStepperDriver::setDirection(int direction){
    digitalWrite2f(dir_pin, (direction<0) ? LOW : HIGH);
}

/*
 * Move the motor a given number of steps.
 * positive to move forward, negative to reverse
 */
void BasicStepperDriver::move(long steps){
    if (steps >= 0){
        setDirection(1);
    } else {
        setDirection(-1);
        steps = -steps;
    }
    /*
     * We currently try to do a 50% duty cycle so it's easy to see.
     * Other option is step_high_min, pulse_duration-step_high_min.
     */
    pulse_duration = step_pulse/2;
    steps_remaining = steps;
    pulse_state = LOW;
    last_step_time = micros() - pulse_duration;
    
    //wait for update tick instead of running here
    /*
    while (steps--){
        digitalWrite2f(step_pin, HIGH);
        unsigned long next_edge = micros() + pulse_duration;
        microWaitUntil(next_edge);
        digitalWrite2f(step_pin, LOW);
        microWaitUntil(next_edge + pulse_duration);
    }
    */
}

/*
 * Move the motor a given number of degrees (1-360)
 */
void BasicStepperDriver::rotate(long deg){
    long steps = deg * motor_steps * (long)microsteps / 360;
    move(steps);
}
/*
 * Move the motor with sub-degree precision.
 * Note that using this function even once will add 1K to your program size
 * due to inclusion of float support.
 */
void BasicStepperDriver::rotate(double deg){
    long steps = deg * motor_steps * microsteps / 360;
    move(steps);
}

/*
 * Enable/Disable the motor by setting a digital flag
 */
void BasicStepperDriver::enable(void){
    if IS_CONNECTED(enable_pin){
        digitalWrite2f(enable_pin, LOW);
    }
}

void BasicStepperDriver::disable(void){
    if IS_CONNECTED(enable_pin){
        digitalWrite2f(enable_pin, HIGH);
    }
}

unsigned BasicStepperDriver::getMaxMicrostep(){
    return BasicStepperDriver::MAX_MICROSTEP;
}

// progresses the current movement
void BasicStepperDriver::tickMovement() {
    if (isMoving()) {
        if (micros() - last_step_time > pulse_duration) {
            
            pulse_state = !pulse_state;
            digitalWrite2f(step_pin, pulse_state);
            
            //only decrement after a complete HIGH and LOW cycle
            if (pulse_state == LOW) {
                steps_remaining--;
            }
            last_step_time = micros();
        }
    }
}


bool BasicStepperDriver::isMoving() {
    return steps_remaining > 0;
}

long BasicStepperDriver::getStepsRemaining() {
    return steps_remaining;
}
