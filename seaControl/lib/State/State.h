//
// Created by Biomech on 2/3/2021.
//

#ifndef SEACONTROL_STATE_H
#define SEACONTROL_STATE_H


enum state {
    stopped,  // 0
    start,    // 1
    axisInit, // 2
    axisInitComplete, //3
    goToStart, // 4
    idle,  // 5
    //down,  // 6
    //up,    // 7
    manUp, // 8
    manDown, //9
    pleaseGoToPos // 10

};

//volatile extern state SEAstate = stopped;
//enum state SEAstate = stopped;

class State {
//public:

};



#endif //SEACONTROL_STATE_H
