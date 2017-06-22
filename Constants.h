#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    Pins
*/

// X-axis
#define PIN_X_STEP          DP54
#define PIN_X_DIR           DP55
#define PIN_X_EN            DP38

// Y-axis
#define PIN_Y_STEP          DP46 // use z axis
#define PIN_Y_DIR           DP48 // use z axis
#define PIN_Y_EN            DP62 // use z axis

// Laser
#define PIN_LASER_POWER     DP8
#define PIN_LASER_TTL       4 // should be far right servo channel, don't use dio2 for PWM


// LEDs
#define PIN_LED_POWER       DP_INVALID  // turns on when board has booted
#define PIN_LED_ACT         DP_INVALID  // blinks when executing gcode
#define PIN_LED_LIFE        13          // pulses while board running (PWM)

// Motors
#define MOTOR_MICROSTEPS    16          // Microsteps per full step
#define STEPS_PER_ROTATION  200L        // Full steps reuired per rotation
#define DEGREES_PER_STEP    1.8         // 1.8 degrees / step
#define STEPS_PER_DEGREE    0.55556     // 1.8 degrees / step

#endif //CONSTANTS_H
