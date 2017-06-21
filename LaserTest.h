#ifndef LASERTEST_H
#define LASERTEST_H
#include "Stepper.h"

stepper::Stepper testStepper1(54, 55, 38);
stepper::Stepper testStepper2(60, 61, 56);

void testSetup() {
  Serial.begin(115200);
  
  testStepper1.enable();
  testStepper2.enable();
  
  Serial.println("Starting.");
}

void testMove(int step, int deg) {
    if (step == 1) {
        testStepper1.moveToDeg(deg);
    } else if (step == 2) {
        testStepper2.moveToDeg(deg);
    }
    Serial.print("Moving ");
    Serial.print(step);
    Serial.print(" to ");
    Serial.println(deg);
}

void testLoop() {
  testMove(1, 0);
  testMove(2, 0);
  delay(1000);
  testMove(1, 90);
  testMove(2, 270);
  delay(1000);
  testMove(1, 180);
  testMove(2, 180);
  delay(1000);
  testMove(1, 270);
  testMove(2, 90);
  delay(1000);
}

#endif //LASERTEST_H
