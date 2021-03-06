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

enum stateType {
    SEA,
    multiDof,
    zHorizontal,
    zRotate,
    alphaRotate,
    multiState
};

class State {
//public:
private:
    enum state myState;
    enum stateType myStateType;
    String stateTypeString;
    long SEADestinationPosition;
    long globalDestinationPosition;
    bool move;
    long stateStartTime;
    int multiDest;
    int zTranslateDestination;
    int zRotateDestination;
    int alphaRotateDestination;
    long currentPosition;
public:
    void setState(enum state setState);
    void goToGlobalPos(long position, float pGain, float iGain, float dGain, int baseSpeed);
    void goToSEAPos(long position, float pGain, float iGain, float dGain, int baseSpeed);
    enum state getState();
    enum stateType getStateType();
    long getSEADest();
    long getGlobalDest();
    void setGlobalDest(long pos);
    void setMove(bool toMoveOrNotToMove);
    bool getMove();
    unsigned long getStateStartTime();
    unsigned long getStateTime();
    void initStartTime();
    void setStateType(enum stateType setStateType);
    boolean stateChange = false;
    void setCurrentPosition(long pos);
    String getStateTypeString();




//    void getMultiDest();
//    void setMultiDest(int dest);
//    int getZTranslateDest();
//    int getZRotateDest();
//    void setZTranslateDest(int dest);
//    void setZRotateDest(int dest);


};

#endif //SEACONTROL_STATE_H
