#ifndef UNITS_H
#define UNITS_H

/*
    --------------------
    Types
    --------------------
*/

/*
    Basic Types
*/

// positions
typedef long dist_mm;
typedef long dist_um;

// distances
typedef unsigned long dist_mm_u;
typedef unsigned long dist_um_u;

// time
typedef unsigned long time_us;
typedef unsigned long time_ms;
typedef unsigned long time_sec;
typedef unsigned long time_min;

// steppers
typedef long step_step;
typedef long step_deg;

/*
    Composite types
*/

// velocity
typedef unsigned long mm_per_min;
typedef unsigned long mm_per_sec;
typedef unsigned long um_per_sec;

// stepper speed
typedef unsigned long step_per_min;
typedef unsigned long step_per_sec;
typedef unsigned long step_per_ms;
typedef unsigned long step_rpm;

// stepper distance
typedef unsigned long step_per_mm;
typedef unsigned long step_per_um;

/*
    Complex types
*/

// decimal
struct dist_mm_d {
    long mm;
    long dec;
};


/*
    -----------------------
    Conversions
    -----------------------
*/

dist_um mmToUm(dist_mm mm);
dist_mm umToMm(dist_um um);
dist_um_u mmToUm_u(dist_mm_u mm);
dist_mm_u umToMm_u(dist_um_u um);

time_ms usToMs(time_us us);
time_us msToUs(time_ms ms);
time_sec minToSec(time_min min);
time_ms secToMs(time_sec sec);
time_min secToMin(time_sec sec);
time_sec msToSec(time_ms ms);

mm_per_min mmPerMin(dist_mm mm, time_min min);
mm_per_sec mmPerSec(dist_mm mm, time_sec sec);
um_per_sec umPerSec(dist_um um, time_sec sec);

step_per_min stepPerMin(step_step step, time_min min);
step_per_sec stepPerSec(step_step step, time_sec sec);
step_per_ms stepPerMs(step_step step, time_ms ms);

step_per_mm stepPerMm(step_step step, dist_mm mm);
step_per_um stepPerUm(step_step step, dist_um um);

dist_mm_d umToMm_dec(dist_um um);

step_step calcStepsForUm(dist_um um, step_per_mm stepsMM);
step_rpm calcRPM(step_per_mm stepMM, mm_per_min mmMin);

#endif //UNITS_H
