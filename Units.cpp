#include "Units.h"
#include "Constants.h"

dist_um mmToUm(dist_mm mm) {return mm / 1000;}
dist_mm umToMm(dist_um um) {return um * 1000;}
dist_um_u mmToUm_u(dist_mm_u mm) {return mm / 1000;}
dist_mm_u umToMm_u(dist_um_u um) {return um * 1000;}

time_sec minToSec(time_min min) {return min * 60;}
time_ms secToMs(time_sec sec) {return sec * 1000;}
time_min secToMin(time_sec sec) {return sec * 60;}
time_sec msToSec(time_ms ms) {return ms * 1000;}

mm_per_min mmPerMin(dist_mm mm, time_min min) {return mm / min;}
mm_per_sec mmPerSec(dist_mm mm, time_sec sec) {return mm / sec;}
um_per_sec umPerSec(dist_um um, time_sec sec) {return um / sec;}

step_step degToStep(step_deg deg) {return (step_step)((float)deg * DEGREES_PER_STEP);}
step_deg stepToDeg(step_step step) {return (step_step)((float)step * STEPS_PER_DEGREE);}

step_per_min stepPerMin(step_step step, time_min min) {return step / min;}
step_per_sec stepPerSec(step_step step, time_sec sec) {return step / sec;}
step_per_ms stepPerMs(step_step step, time_ms ms) {return step / ms;}

step_per_mm stepPerMm(step_step step, dist_mm mm) {return step / mm;}
step_per_um stepPerUm(step_step step, dist_um um) {return step / um;}

dist_mm_d umToMm_dec(dist_um um) {
    dist_mm_d mmD;
    mmD.mm = um / 1000;
    mmD.dec = um % 1000;
    return mmD;
}

step_step calcStepsForUm(dist_um um, step_per_mm speed) {
    return (speed * um) / 1000L;
}
step_rpm calcRPM(step_per_mm stepMM, mm_per_min mmMin) {
    return (stepMM * mmMin) / STEPS_PER_ROTATION;
}
