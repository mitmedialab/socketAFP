//
// Created by Aaron Jaeger on 2/3/2021.
//

#include "State.h"


void State::setState(enum state setState) {
    if(setState != this->myState ){
        this->stateStartTime = millis();
        this->stateChange = true;

    }

    this->myState = setState;

}

void State::goToGlobalPos(long position, float pGain, float iGain, float dGain, int baseSpeed) {
    this->globalDestinationPosition = position;
}

void State::goToSEAPos(long position, float pGain, float iGain, float dGain, int baseSpeed) {

}

//getters

enum state State::getState() {
    return this->myState;
}

long State::getSEADest() {
    return this->SEADestinationPosition;
}

long State::getGlobalDest() {
    return this->globalDestinationPosition;
}

void State::setGlobalDest(long pos) {
   this->globalDestinationPosition = pos;
}


bool State::getMove() {
    return this->move;
}

void State::setMove(bool toMoveOrNotToMove) {
    this->move = toMoveOrNotToMove;
}

unsigned long State::getStateStartTime() {
    return stateStartTime;
}

unsigned long State::getStateTime() {
    unsigned long stateTime = millis() - stateStartTime;
    return stateTime;
}

void State::initStartTime() {
    stateStartTime = millis();
}

enum stateType State::getStateType() {
    return this->myStateType;
}

void State::setStateType(enum stateType setStateType) {
    this->myStateType = setStateType;
    /*
     * create string version... this is kinda gross
     */
    switch (setStateType) {
        case SEA:
            this->stateTypeString = "SEA";
            break;
        case multiDof:
            this->stateTypeString = "MultiDof";
            break;
        case zHorizontal:
            this->stateTypeString = "zHorizontal";
            break;
        case zRotate:
            this->stateTypeString = "zRotate";
            break;
        case alphaRotate:
            this->stateTypeString = "alphaRotate";
            break;
        case multiState:
            this->stateTypeString = "multiState";
            break;
    }
}

void State::setCurrentPosition(long pos) {
    this->currentPosition = pos;
}

String State::getStateTypeString() {
    return this->stateTypeString;
}



