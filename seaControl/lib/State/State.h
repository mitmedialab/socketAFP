//
// Created by Biomech on 2/3/2021.
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
    pleaseGoToPos, // 11
    posTwo

};

//volatile extern state SEAstate = stopped;
//enum state SEAstate = stopped;

class State {
//public:

};



#endif //SEACONTROL_STATE_H
