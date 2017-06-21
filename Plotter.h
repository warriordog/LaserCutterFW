#ifndef PLOTTER_H
#define PLOTTER_H

namespace plotter {
    /*
        Settings
    */
    
    //movement speed of axis (mm / min)
    int xSpeed = 0;
    int ySpeed = 0;

    /*
        Current state
    */

    //current location of axis (um)
    long xLocation = 0;
    long yLocation = 0;
    
    //if an axis is currently moving
    bool xMoving = false;
    bool yMoving = false;
    
    /*
        Movement
    */
    
    //target of current movement (um)
    long xTarget = 0;
    long yTarget = 0;
    
    //location of axis at start of movement (um)
    long xStart = 0;
    long yStart = 0;
    
    //time that target location was reached
    unsigned long timeArrived = 0;
    
    //time that the current movement started
    unsigned long movementStartTime = 0;
    
    /*
        Motors
    */
    
    Stepper* xStepper = nullptr;
    Stepper* yStepper = nullptr;
    
    bool hasXMotor() {return xStepper != nullptr;}
    bool hasYMotor() {return yStepper != nullptr;}
    
    void setXMotor(Stepper* stepper) {xStepper = stepper;}
    void setYMotor(Stepper* stepper) {yStepper = stepper;}
    
    void setTargetX(int target);
    void setTargetY(int target);
    void setTarget(int x, int y) {setTargetX(x); setTargetY(y);}
    
    void setXVelocity(int vel) {xVelocity = vel;}
    void setYVelocity(int vel) {yVelocity = vel;}
    
    bool isMovingX() {return xMoving;}
    bool isMovingY() {return yMoving;}
    bool isMoving() {return isMovingX() || isMovingY();}
    
    unsigned long getTimeArrived() {return timeArrived;}
    
    void updateMovement();
}

#endif //PLOTTER_H
