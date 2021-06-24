//
// Created by Biomech on 6/22/2021.
//

#include "MultiState.h"

State MultiState::getZTState() {
    return this->zTranslateState;
}

State MultiState::getZRState() {
    return this->zRotateState;
}

State MultiState::getAState() {
    return this->AlphaState;
}

void MultiState::setZTState(State setState) {
    this->zTranslateState = setState;
}

void MultiState::setZRState(State setState) {
    this->zRotateState = setState;
}

void MultiState::setAState(State setState) {
    this->AlphaState = setState;
}


State MultiState::buildState(long pos, boolean active, stateType newStateType) {
    State tempState;
    if(active){
        tempState.setMove(true);
        tempState.setStateType(zHorizontal);
        tempState.setGlobalDest(pos);
        tempState.setState(GoToPos);
        tempState.stateChange = true;
    }
    else{
        tempState.setMove(false);
        tempState.setState(idle);
    }
    return tempState;
}

State MultiState::getGeneralMultiState() {
    this->generalMultiState.setState(idle);
    return this->generalMultiState;
}
