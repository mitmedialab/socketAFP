//
// Created by Biomech on 6/14/2021.
//

#include "SEAStateMachine.h"

State SEAStateMachine::SEAState_Setup(){
    SEAstate.setState( inSetup);
    Serial.begin(115200);

    seaParams.yEnc.setInitConfig();
    seaParams.yEnc.init();
    seaParams.yEnc.write(0);

    seaParams.SEAenc.setInitConfig();
    seaParams.SEAenc.EncConfig.revolutionCountCondition = DISABLE;
    seaParams.SEAenc.EncConfig.enableModuloCountMode = DISABLE;
    seaParams.SEAenc.init();
    seaParams.SEAenc.write(0);

    //SEA Motor setup
    pinMode(seaParams.yPWM, OUTPUT);
    pinMode(seaParams.yEnable, OUTPUT);
    pinMode(seaParams.yDirection, OUTPUT);

    //SEA limit switch setup;
    pinMode(yUpLimitButtonPin, INPUT_PULLUP);
    pinMode(yDownLimitButtonPin, INPUT_PULLUP);

    //UI buttons
    pinMode(startStopButton, INPUT_PULLUP);
    pinMode(manUpButton, INPUT_PULLUP);
    pinMode(manDownButton, INPUT_PULLUP);

    //encoder index
    pinMode(seaParams.yEncIndex, INPUT);
    pinMode(seaParams.SEAEncIndex, INPUT);

    SEAstate.setState( stopped);

    SEAMotor.motorControlInit(seaParams.yPWM, seaParams.yEnable, seaParams.yDirection);
    SEAOutgoing.generalMessage(SEAstate.getState(), "end setup");
}

/*
 * behavior when state is "stopped"
 * disable motor
 * set state to idle
 */
State SEAStateMachine::SEAState_stopped() {
    SEAMotor.driveYMotor(0, false, this->yEncPos);

    State tempState = this->SEAstate;
    tempState.setState( idle);
    return tempState;
}


/*
 * behavior when state is "idle"
 * check serial
 * check manual buttons
 */
State SEAStateMachine::SEAState_idle() {


//    if(!printedStop){
//        outgoing.generalMessage(SEAstate.getState(), "stopped");
////                Serial.println("stopped");
//        printedStop = true;
//    }

    /* check the serial
     *  if there is a message, parse, convert to state
     * reply to computer with the message that was received
     * return the new state.
     */
    if(Serial.available()){
        SEAIncoming.checkSerial(); // this includes a loop, should block until a new line
        if(SEAIncoming.checkComplete()) {
            SEAIncoming.readIncomingJson();

            String checkSerAgain = String(Serial.available());
            SEAOutgoing.generalMessage(SEAstate.getState(), "State from incoming message");
            SEAOutgoing.generalMessage(SEAstate.getState(), ("Serial available: " + checkSerAgain));
//            printedStop = false;
//            printedIdle = false;
            return SEAIncoming.getState();

        }
    }

    else{
        State tempState = this->SEAstate;
        tempState.setState(manDrive(tempState.getState()));
        return tempState;
    }
}

/*
 *  manual up state function
 */

State SEAStateMachine::SEAState_manUp() {
    // drive motor: slowUp, enabled, up
    State tempState = this->SEAstate;
    SEAMotor.driveYMotor(manUpSpeed, true, this->yEncPos);
    tempState.setState(manDrive(tempState.getState()));
    return tempState;
}

/*
 * Manual down state function
 */

State SEAStateMachine::SEAState_manDown() {
    // drive motor, slowDown, enabled, down
    State tempState = this->SEAstate;
    SEAMotor.driveYMotor(manDownSpeed, true, this->yEncPos);
    tempState.setState( manDrive(tempState.getState()));
    return tempState;
}

/*
 * reaction to large "start" button
 * if the machine isn't initialized, set state to axis init
 * if the machine is past threshold, reinit
 * else: go to idle TODO: do something else... lol 
 */
State SEAStateMachine::SEAState_start() {
    State tempState = this->SEAstate;

    // run init if first init hasnt happened.
    if(!seaParams.firstInit){
        tempState.setState( axisInit);
    }
        // reinitialize encoder
    else if( yEncPos < seaParams.encoderReInitThreshold) {
        tempState.setState( axisInit);
    }
        // move onto next state
    else {
        tempState.setState( idle);
    }

    return tempState;
}

State SEAStateMachine::SEAState_axisInit() {
    return State();
}

State SEAStateMachine::SEAState_axisInitComplete() {
    return State();
}

State SEAStateMachine::SEASate_GoToPos() {
    return State();
}
