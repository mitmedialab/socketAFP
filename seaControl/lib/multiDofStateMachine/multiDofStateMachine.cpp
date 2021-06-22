//
// Created by Biomech on 6/16/2021.
//

#include "multiDofStateMachine.h"

State multiDofStateMachine::MultiDof_Setup() {

    Serial.begin(115200);
    Wire.begin();

    mParams.tic1.exitSafeStart();
    mParams.tic1.setCurrentLimit(1500);
    mParams.tic1.setStepMode(TicStepMode::Microstep2);

    mParams.tic2.exitSafeStart();
    mParams.tic2.setCurrentLimit(1500);
    mParams.tic2.setStepMode(TicStepMode::Microstep2);

    State tempState = this -> MultiDofState;
    tempState.setState(idle);
    return tempState;
}

State multiDofStateMachine::MultiDof_stopped() {
    return State();
}

State multiDofStateMachine::MultiDof_idle() {
    State tempState = MultiDofState;
    if(Serial.available()){
        MultiDofIncoming.checkSerial(); // this includes a loop, should block until a new line
        if(MultiDofIncoming.checkComplete()) {
            MultiDofIncoming.readIncomingJson();

            String checkSerAgain = String(Serial.available());
            MultiDofOutgoing.generalMessage(tempState.getState(), "State from incoming message");
            MultiDofOutgoing.generalMessage(tempState.getState(), ("Serial available: " + checkSerAgain));
            this->myMultiState = MultiDofIncoming.getMultiState();
            return MultiDofIncoming.getState();

        }
    }

    return tempState;
}

State multiDofStateMachine::ZTranslate_idle() {
    return State();
}

/*
 * got to z translate position
 * if first time, set target position
 * after, resetCommandTimout until done
 * then go back to idle
 */
State multiDofStateMachine::ZTranslate_gotToPos() {
    State tempState = this->ZTranslateState;
    if(tempState.stateChange){
        mParams.tic1.setTargetPosition(tempState.getZTranslateDest());
        tempState.stateChange = false;
    }
    else if(mParams.tic1.getCurrentPosition() != tempState.getZTranslateDest()){
        mParams.tic1.resetCommandTimeout();
    }
    else{
        tempState.setState(idle);
    }
    return tempState;
}

State multiDofStateMachine::ZTranslate_stopped() {
    return State();
}


State multiDofStateMachine::ZRotate_goToPos() {
    return State();
}

state multiDofStateMachine::ZRotate_stopped() {
    return manUp;
}

State multiDofStateMachine::ZRotate_idle() {
    return State();
}

State multiDofStateMachine::AlphaRotate_goToPos() {
    return State();
}

State multiDofStateMachine::AlphaRotate_stopped() {
    return State();
}

State multiDofStateMachine::SecondHeaterHigh() {
    return State();
}

State multiDofStateMachine::SecondHeaterLow() {
    return State();
}

void multiDofStateMachine::runZtranslateStateMachine() {
    switch(this->ZTranslateState.getState()){
        case stopped:
            break;
        case idle:
            break;
        case axisInit:
            break;
        case goToStart:
            break;
        case GoToPos:
            ZTranslateState = ZRotate_goToPos();
            MultiDofOutgoing(ZTranslateState.getState(), )
            break;
    }

}

void multiDofStateMachine::runZrotateStateMachine() {
    switch(this->ZRotationState.getState()){
        case stopped:
            break;
        case idle:
            break;
        case axisInit:
            break;
        case goToStart:
            break;
        case GoToPos:
            break;
    }
}

void multiDofStateMachine::runAlphaRotateStateMachine() {
    switch(this->AlphaState.getState()){
        case stopped:
            break;
        case idle:
            break;
        case axisInit:
            break;
        case goToStart:
            break;
        case GoToPos:
            break;
    }
}

void multiDofStateMachine::runMultiDofState() {
    State tempState = MultiDof_idle();
    if(tempState.getState() != 0){
        MultiDofOutgoing.generalMessage(MultiDofState.getState(), String(tempState.getState()),
                                        String(tempState.getStateType())) ;
    }



}

void multiDofStateMachine::updateZTranslateGlobalPos(int newPos) {

}

void multiDofStateMachine::updateZRotateGlobalPos(int newPos) {

}

