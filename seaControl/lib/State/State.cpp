//
// Created by Aaron Jaeger on 2/3/2021.
//

#include "State.h"


void State::setState(enum state setState) {
    if(setState != this->myState){
        stateStartTime = millis();
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

bool State::getMove() {
    return this->move;
}

void State::setMove(bool toMoveOrNotToMove) {
    this->move = toMoveOrNotToMove;
}

long State::getStateStartTime() {
    return stateStartTime;
}

