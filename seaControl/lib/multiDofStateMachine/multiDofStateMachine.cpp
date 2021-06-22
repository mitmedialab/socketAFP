//
// Created by Biomech on 6/16/2021.
//

#include "multiDofStateMachine.h"

State multiDofStateMachine::MultiDof_Setup() {

    Serial.begin(115200);

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
//            printedStop = false;
//            printedIdle = false;
            return MultiDofIncoming.getState();

        }
    }

    return tempState;
}

State multiDofStateMachine::ZTranslate_idle() {
    return State();
}
State multiDofStateMachine::ZTranslate_gotToPos() {
    return State();
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

}

void multiDofStateMachine::runZrotateStateMachine() {

}

void multiDofStateMachine::runAlphaRotateStateMachine() {

}

void multiDofStateMachine::runMultiDofState() {
    MultiDof_idle();
}

