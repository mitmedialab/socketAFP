//
// Created by Biomech on 2/5/2021.
//

#ifndef SEACONTROL_ACTUATORCONTROL_H
#define SEACONTROL_ACTUATORCONTROL_H

#include "../State/State.h"
#include "Arduino.h"

// global variable initializations
const int stoppedSpeed = 0;
//const int manUpSpeed = 35; //80 is good at 12v
//const int manDownSpeed = -29;  //-50 is good at 12v
//const int initUpSpeed = 33;  //40 is good at 12v
//const int touchHardStop = 33; //32 is good at 12v
const int manUpSpeed = 100; //80 is good at 12v
const int manDownSpeed = -50;  //-50 is good at 12v
const int initUpSpeed = 95;  //40 is good at 12v
const int touchHardStop = 90; //32 is good at 12v
const int errorThreshold = 100;
//const int yEncA = 2;
const double pGain = 0.05;
const double dGain = 100;
const double iGain = 0.01;
class ActuatorControl {
    uint8_t pwmPin;
    uint8_t enablePin;
    uint8_t dirPin;
    //unsigned long oldEncoderPosition;
    double pError =0;
    double dError =0;
    double iError =0;
public:
    void motorControlInit(uint8_t pwm, uint8_t enable,uint8_t direction);
    void driveYMotor(int motorSpeed, bool enable, long yEncoderPos);
    void initYMotor();
    enum state pdControl(long desiredPosition, long yEcoderPos, long currentTime, int basePWM, enum state currentState);
};

#endif //SEACONTROL_ACTUATORCONTROL_H
