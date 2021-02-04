//
// Created by Biomech on 2/3/2021.
//

#ifndef SEACONTROL_BUTTONBEHAVIOR_H
#define SEACONTROL_BUTTONBEHAVIOR_H

#include "../State/State.h"

// limit switch pins
const int yUpLimitButtonPin = 15;
const int yDownLimitButtonPin = 16;

// UI button pins
const int startStopButton = 17;
const int manUpButton = 32;
const int manDownButton = 33;

//long buttonLockOut = 0;

enum state manDrive(enum state currentState);
enum state checkLimits(enum state currentState);
enum state startStop(enum state currentState);

class ButtonBehavior {
public:

};


#endif //SEACONTROL_BUTTONBEHAVIOR_H
