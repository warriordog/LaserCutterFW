#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    Pins
*/

// X-axis
#define PIN_X_STEP          DP46 // use z axis
#define PIN_X_DIR           DP48 // use z axis
#define PIN_X_EN            DP62 // use z axis

// Y-axis
#define PIN_Y_STEP          DP60
#define PIN_Y_DIR           DP61
#define PIN_Y_EN            DP56

// Laser
#define PIN_LASER_POWER     DP8
#define PIN_LASER_TTL       4 // should be far right servo channel, don't use dio2 for PWM


// LEDs
#define PIN_LED_POWER       DP_INVALID  // turns on when board has booted
#define PIN_LED_ACT         DP_INVALID  // blinks when executing gcode
#define PIN_LED_LIFE        13          // pulses while board running (PWM)

// Motors
#define MOTOR_MICROSTEPS    16          // Microsteps enabled
#define STEPS_PER_ROTATION  200L        // Full steps reuired per rotation

#endif //CONSTANTS_H
