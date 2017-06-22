#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    Pins
*/

// X-axis
#define PIN_X_STEP      DP54
#define PIN_X_DIR       DP55
#define PIN_X_EN        DP38

// Y-axis
#define PIN_Y_STEP      DP46 // use z axis
#define PIN_Y_DIR       DP48 // use z axis
#define PIN_Y_EN        DP62 // use z axis

// Laser
#define PIN_LASER_POWER     DP8
#define PIN_LASER_TTL       4 // should be far right servo channel, don't use dio2 for PWM

#endif //CONSTANTS_H
