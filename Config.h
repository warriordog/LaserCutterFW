#ifndef CONFIG_H
#define CONFIG_H

/*
  Configuration values
*/

// min and max coordinates for the X axis (mm)
#define X_AXIS_MIN          0
#define X_AXIS_MAX          900     //just under 3 feet

// min and max coordinates for the Y axis (mm)
#define Y_AXIS_MIN          0
#define Y_AXIS_MAX          600     //just under 2 feet

// steps per millimeter
#define X_STEPS_PER_MM      300
#define Y_STEPS_PER_MM      300

// default feed rates (mm / min)
#define DEF_X_FEED          1000
#define DEF_Y_FEED          1000

// number of gcode instructions to queue
#define GCODE_QUEUE_SIZE    16

// Serial baud rate
#define SERIAL_BAUD         115200

// Max length of a gcode instruction
#define MAX_GCODE_LENGTH    64

#endif //CONFIG_H
