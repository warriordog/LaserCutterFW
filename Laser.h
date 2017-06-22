#ifndef LASER_H
#define LASER_H

namespace laser {
    void setup();

    void laserPowerOff();
    void laserPowerOn();
    bool isLaserOn();
    
    void setLaserLevel(int level);
    int getLaserLevel();
}

#endif
