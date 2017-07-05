#ifndef CONFIG_H
#define CONFIG_H

/*
  Configuration values
*/

#define FIRMWARE_VERSION    "LaserFW 0.5.0"

// min and max coordinates for the X axis (mm)
#define X_AXIS_MIN          0
#define X_AXIS_MAX          900     //just under 3 feet

// min and max coordinates for the Y axis (mm)
#define Y_AXIS_MIN          0
#define Y_AXIS_MAX          600     //just under 2 feet

// microsteps per millimeter
#define X_STEPS_PER_MM      960L //(60 * 16 microsteps), 5760 is full turn
#define Y_STEPS_PER_MM      960L //(60 * 16 microsteps), 5760 is full turn

// default feed rates (mm / m)
#define DEF_X_FEED          60L
#define DEF_Y_FEED          60L

// max feed rates (mm / m)
#define MAX_X_FEED          180L
#define MAX_Y_FEED          180L

// motor RPM
#define MAX_RPM             250L
#define DEFAULT_RPM         120L

// Serial baud rate
#define SERIAL_BAUD         115200

// number of gcode instructions to queue
#define GCODE_QUEUE_SIZE    16

// Max length of a gcode instruction
#define MAX_GCODE_LENGTH    64

// Maximum time in ms that laser can stay in one place
#define LASER_DWELL_LIMIT   4000

// LED blink times
    // Safety fault blink
#define LED_SAFETY_DURATION 10000   // 10 seconds
#define LED_SAFETY_INTERVAL 500     // every half second
    // gcode error blink
#define LED_ERROR_DURATION  5000    // 5 seconds
#define LED_ERROR_INTERVAL  1000    // every second
    // gcode executed blink
#define LED_ACT_DURATION    100     // 1/10 of a second

// Time for life LED to complete one pulse-fade cycle (ms)
#define LED_LIFE_INTERVAL   2000L   // 2 seconds

// Default time to disable laser safety in M888
#define DEFAULT_M888_TIME   10      // 10 seconds

#endif //CONFIG_H
