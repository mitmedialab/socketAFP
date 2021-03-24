//
// Created by Biomech on 2/3/2021.
//

#ifndef SEACONTROL_BUTTONBEHAVIOR_H
#define SEACONTROL_BUTTONBEHAVIOR_H

#include "../State/State.h"

// limit switch pins
const int yUpLimitButtonPin = 33;
const int yDownLimitButtonPin = 34;

// UI button pins
const int startStopButton = 2;
const int manUpButton = 1;
const int manDownButton = 0;

//long buttonLockOut = 0;

enum state manDrive(enum state currentState);
enum state checkLimits(enum state currentState);
enum state startStop(enum state currentState);

class ButtonBehavior {
public:

};


#endif //SEACONTROL_BUTTONBEHAVIOR_H
