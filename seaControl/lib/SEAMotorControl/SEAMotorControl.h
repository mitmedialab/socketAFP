//
// Created by Biomech on 2/3/2021.
//

#ifndef SEACONTROL_SEAMOTORCONTROL_H
#define SEACONTROL_SEAMOTORCONTROL_H

#include "../State/State.h"

const int yPWM = 6;
const int yEnable = 5;
const int yDirection = 4;

// global variable initializations
const int stoppedSpeed = 0;
const int manUpSpeed = 35; //80 is good at 12v
const int manDownSpeed = -29;  //-50 is good at 12v
const int initUpSpeed = 33;  //40 is good at 12v
const int touchHardStop = 33; //32 is good at 12v


class SEAMotorControl {
//    enum state testState = stopped;
public:
    void driveYMotor(int motorSpeed, bool enable, long yEconderPos);
};

#endif //SEACONTROL_SEAMOTORCONTROL_H
