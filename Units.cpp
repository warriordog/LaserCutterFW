#include "Units.h"
#include "Constants.h"

dist_um mmToUm(dist_mm mm) {return mm * 1000L;}
dist_mm umToMm(dist_um um) {return um / 1000L;}
dist_um_u mmToUm_u(dist_mm_u mm) {return mm * 1000L;}
dist_mm_u umToMm_u(dist_um_u um) {return um / 1000L;}

time_ms usToMs(time_us us) {return us / 1000L;}
time_us msToUs(time_ms ms) {return ms * 1000L;}
time_sec minToSec(time_min min) {return min * 60L;}
time_ms secToMs(time_sec sec) {return sec * 1000L;}
time_min secToMin(time_sec sec) {return sec / 60L;}
time_sec msToSec(time_ms ms) {return ms / 1000L;}

mm_per_min mmPerMin(dist_mm mm, time_min min) {return mm / min;}
mm_per_sec mmPerSec(dist_mm mm, time_sec sec) {return mm / sec;}
um_per_sec umPerSec(dist_um um, time_sec sec) {return um / sec;}

step_per_min stepPerMin(step_step step, time_min min) {return step / min;}
step_per_sec stepPerSec(step_step step, time_sec sec) {return step / sec;}
step_per_ms stepPerMs(step_step step, time_ms ms) {return step / ms;}

step_per_mm stepPerMm(step_step step, dist_mm mm) {return step / mm;}
step_per_um stepPerUm(step_step step, dist_um um) {return step / um;}

dist_mm_d umToMm_dec(dist_um um) {
    dist_mm_d mmD;
    mmD.mm = um / 1000L;
    mmD.dec = um % 1000L;
    return mmD;
}

step_step calcStepsForUm(dist_um um, step_per_mm stepsMM) {
    return (stepsMM * um) / 1000L;
}
step_rpm calcRPM(step_per_mm stepMM, mm_per_min mmMin) {
    return (stepMM * mmMin) / STEPS_PER_ROTATION;
}
