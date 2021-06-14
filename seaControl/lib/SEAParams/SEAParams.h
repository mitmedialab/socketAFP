//
// Created by Biomech on 6/14/2021.
//

#ifndef SEACONTROL_SEAPARAMS_H
#define SEACONTROL_SEAPARAMS_H

#include "../QuadEncoder/QuadEncoder.h"


class SEAParams {
public:
    // maxon motor pins
    const uint8_t  yPWM = 25;
    const uint8_t  yEnable = 24;
    const uint8_t  yDirection = 12;

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
    const int yEncIndex = 29; //at 0.5 in
    const int yEncA = 4;
    const int yEncB = 5;

    //sea encoder pins
    const int SEAEncIndex = 32; //at 0.5 in
    const int SEAEncA = 31;
    const int SEAEncB = 30;

    //encoder initializations
    QuadEncoder yEnc = QuadEncoder(1, yEncA, yEncB);
    QuadEncoder SEAenc= QuadEncoder(2, SEAEncA, SEAEncB);


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
};


#endif //SEACONTROL_SEAPARAMS_H
