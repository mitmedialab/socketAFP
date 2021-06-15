//
// Created by Aaron Jaeger on 2/3/2021.
//

#ifndef SEACONTROL_STATE_H
#define SEACONTROL_STATE_H

#include "Arduino.h"

enum state {
    inSetup,    // 0
    stopped,  // 1
    start,    // 3
    axisInit, // 3
    axisInitComplete, //4
    goToStart, // 5
    idle,  // 6
    //down,  // 7
    //up,    // 8
    manUp, // 7
    manDown, //8
    GoToPos, // 9
    anotherState
//    posTwo

};

class State {
//public:
private:
    enum state myState;
    long SEADestinationPosition;
    long globalDestinationPosition;
    bool move;
    long stateStartTime;
public:
    void setState(enum state setState);
    void goToGlobalPos(long position, float pGain, float iGain, float dGain, int baseSpeed);
    void goToSEAPos(long position, float pGain, float iGain, float dGain, int baseSpeed);
    enum state getState();
    long getSEADest();
    long getGlobalDest();
    void setMove(bool toMoveOrNotToMove);
    bool getMove();
    unsigned long getStateStartTime();
    unsigned long getStateTime();
    void initStartTime();
    boolean stateChange = false;
};

#endif //SEACONTROL_STATE_H
