#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    Pins
*/

// X-axis
#define PIN_X_STEP          46//DP46 // use z axis
#define PIN_X_DIR           48//DP48 // use z axis
#define PIN_X_EN            62//DP62 // use z axis

// Y-axis
#define PIN_Y_STEP          60//DP60
#define PIN_Y_DIR           61//DP61
#define PIN_Y_EN            56//DP56

// Laser
#define PIN_LASER_POWER     8//DP8
#define PIN_LASER_TTL       5// should be 2nd to far right servo channel, don't use dio2 for PWM


// LEDs
#define PIN_LED_POWER       -1//DP_INVALID  // turns on when board has booted
#define PIN_LED_ACT         -1//DP_INVALID  // blinks when executing gcode
#define PIN_LED_LIFE        13          // pulses while board running (PWM)

// Motors
#define MOTOR_MICROSTEPS    16          // Microsteps enabled
#define STEPS_PER_ROTATION  200L        // Full steps reuired per rotation

#endif //CONSTANTS_H
