//
// Created by Biomech on 6/16/2021.
//

#include "multiDofStateMachine.h"

/*
 * device setup helper function
 */
State multiDofStateMachine::MultiDof_Setup() {

    Serial.begin(115200);
    Wire.begin();
    mParams.tic1.haltAndSetPosition(0);
    mParams.tic1.exitSafeStart();
    mParams.tic1.setCurrentLimit(1500);
    mParams.tic1.setStepMode(TicStepMode::Microstep2);

    mParams.tic2.haltAndSetPosition(0);
    mParams.tic2.exitSafeStart();
    mParams.tic2.setCurrentLimit(1500);
    mParams.tic2.setStepMode(TicStepMode::Microstep2);

    State tempState = this -> MultiDofState;
    tempState.setState(idle);
    return tempState;
}
/*
 * stop everything! helper function
 */
State multiDofStateMachine::MultiDof_stopped() {
    return State();
}

/*
 * helper function to manage incoming messages
 */
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
            return myMultiState.getGeneralMultiState();

        }
    }

    return tempState;
}

/*
 * Helper functions for the Z translational stage state machine
 */

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
    State tempState = this->myMultiState.getZTState();
    if(tempState.stateChange){
        mParams.tic1.clearDriverError();
        mParams.tic1.setTargetPosition(tempState.getGlobalDest());
        mParams.tic1.resetCommandTimeout();
        MultiDofOutgoing.generalMessage(tempState.getState(), "target position set", "good luck");
        tempState.stateChange = false;
    }
    else if(mParams.tic1.getCurrentPosition() != tempState.getGlobalDest()){
        mParams.tic1.resetCommandTimeout();
//        MultiDofOutgoing.generalMessage(tempState.getState(), String(mParams.tic1.getCurrentPosition()),
//                                        "current position of stepper");
    }
    else{

        uint32_t start = millis();
        do
        {
            mParams.tic1.resetCommandTimeout();
        } while ((uint32_t)(millis() - start) <= 100);
        tempState.setState(idle);
    }
    return tempState;
}

State multiDofStateMachine::ZTranslate_stopped() {
    return State();
}

/*
 * helper functions for spindle state machine
 */

State multiDofStateMachine::ZRotate_goToPos() {
    State tempState = this->myMultiState.getZRState();
    if(tempState.stateChange){
        mParams.tic2.clearDriverError();
        mParams.tic2.setTargetPosition(tempState.getGlobalDest());
        mParams.tic2.resetCommandTimeout();
        MultiDofOutgoing.generalMessage(tempState.getState(), "target position set", "good luck");
        tempState.stateChange = false;
    }
    else if(mParams.tic2.getCurrentPosition() != tempState.getGlobalDest()){
        mParams.tic2.resetCommandTimeout();
//        MultiDofOutgoing.generalMessage(tempState.getState(), String(mParams.tic1.getCurrentPosition()),
//                                        "current position of stepper");
    }
    else{

        uint32_t start = millis();
        do
        {
            mParams.tic2.resetCommandTimeout();
        } while ((uint32_t)(millis() - start) <= 100);
        tempState.setState(idle);
    }
    return tempState;

}

state multiDofStateMachine::ZRotate_stopped() {
    return manUp;
}

State multiDofStateMachine::ZRotate_idle() {
    return State();
}

/*
 * Helper functions for alpha dof state machine
 */
State multiDofStateMachine::AlphaRotate_goToPos() {
    return State();
}

State multiDofStateMachine::AlphaRotate_stopped() {
    return State();
}

/*
 * helper functions for heater.
 */
State multiDofStateMachine::SecondHeaterHigh() {
    return State();
}

State multiDofStateMachine::SecondHeaterLow() {
    return State();
}

/*
 * state machine for Z translational axis
 */
void multiDofStateMachine::runZtranslateStateMachine() {
    State zTempState = this->myMultiState.getZTState();
    mParams.tic1.resetCommandTimeout();
    switch(zTempState.getState()){
//    switch(this->ZTranslateState.getState()){
        case stopped:
            break;
        case idle:
            break;
        case axisInit:
            break;
        case goToStart:
            break;
        case GoToPos:
            zTempState = ZTranslate_gotToPos();
            this->myMultiState.setZTState(zTempState);
            break;
    }

}

/*
 * State machine for spindle / z rotational axis
 */
void multiDofStateMachine::runZrotateStateMachine() {
    State zTempState = this->myMultiState.getZRState();
    mParams.tic2.resetCommandTimeout();
    switch(zTempState.getState()){
        case stopped:
            break;
        case idle:
            break;
        case axisInit:
            break;
        case goToStart:
            break;
        case GoToPos:
            zTempState = ZRotate_goToPos();
            this->myMultiState.setZRState(zTempState);
            break;
    }
}

/*
 * State machine for alpha dof
 */
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

/*
 * device manager state machine
 */
void multiDofStateMachine::runMultiDofState() {
    State tempState = MultiDof_idle();
    if(tempState.getState() != 0){
//        MultiDofOutgoing.generalMessage(MultiDofState.getState(),
//                                        String(myMultiState.getZTState().getState()),
//                                        String(myMultiState.getZTState().getStateType()));
    }

    this->runZtranslateStateMachine();
    this->runZrotateStateMachine();

}

void multiDofStateMachine::updateZTranslateGlobalPos(int newPos) {

}

void multiDofStateMachine::updateZRotateGlobalPos(int newPos) {

}

