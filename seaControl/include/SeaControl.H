//
// Created by Biomech on 2/2/2021.
//

#ifndef SEACONTROL_SEACONTROL_H
#define SEACONTROL_SEACONTROL_H

#endif //SEACONTROL_SEACONTROL_H

//pin initaializations:

// maxon motor pins
const uint8_t  yPWM = 6;
const uint8_t  yEnable = 5;
const uint8_t  yDirection = 4;


// limit switch pins
//const int yUpLimitButtonPin = 15;
//const int yDownLimitButtonPin = 16;
//
// UI button pins
//const int startStopButton = 17;
//const int manUpButton = 32;
//const int manDownButton = 33;

//y encoder pins
// encoder is 2000 Lines per inch, 7 inches long
const int yEncIndex = 22; //at 0.5 in
const int yEncA = 2;
const int yEncB = 3;

//sea encoder pins
const int SEAEncIndex = 28; //at 0.5 in
const int SEAEncA = 20;
const int SEAEncB = 21;
//encoder initializations
Encoder yEnc(yEncA, yEncB);
Encoder SEAenc(SEAEncA, SEAEncB);

//
// global variable initializations
//const int stoppedSpeed = 0;
//const int manUpSpeed = 35; //80 is good at 12v
//const int manDownSpeed = -29;  //-50 is good at 12v
//const int initUpSpeed = 33;  //40 is good at 12v
//const int touchHardStop = 33; //32 is good at 12v
const long encoderReInitThreshold = -2000;
int testMotorSpeed = 30;
unsigned long stateStart;
const long stateLimit = 3000;
long yEncOld =0;
long SEAencOld = 0;
//long buttonLockOut = 0;

bool yDownLimit = false;
bool yUpLimit = false;
bool yIndexHit = false;
bool firstInit = false;

struct controlError {
    float error;
    float dError;
    float pwm;
};



