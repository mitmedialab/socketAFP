//
// Created by Biomech on 6/22/2021.
//

#ifndef SEACONTROL_MULTISTATE_H
#define SEACONTROL_MULTISTATE_H
#include "../State/State.h"

class MultiState {
private:
    State zTranslateState;
    State zRotateState;
    State AlphaState;
    State generalMultiState;
public:
    State getZTState();
    State getZRState();
    State getAState();
    State getGeneralMultiState();
    void setZTState(State setState);
    void setZRState(State setState);
    void setAState(State setState);

    State buildState(int pos, boolean active);
};


#endif //SEACONTROL_MULTISTATE_H
