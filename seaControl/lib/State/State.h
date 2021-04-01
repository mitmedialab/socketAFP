//
// Created by Aaron Jaeger on 2/3/2021.
//

#ifndef SEACONTROL_STATE_H
#define SEACONTROL_STATE_H


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
    manUp, // 9
    manDown, //10
    GoToPos, // 11
    posTwo

};

//volatile extern state SEAstate = stopped;
//enum state SEAstate = stopped;

class State {
//public:
private:
    enum state myState;
    long SEADestinationPosition;
    long globalDestinationPosition;
    bool move;
public:
    void createState(enum state setState);
    void goToGlobalPos(long position, float pGain, float iGain, float dGain, int baseSpeed);
    void goToSEAPos(long position, float pGain, float iGain, float dGain, int baseSpeed);
    enum state getState();
    long getSEADest();
    long getGlobalDest();
    void setMove(bool toMoveOrNotToMove);
    bool getMove();
};

#endif //SEACONTROL_STATE_H
