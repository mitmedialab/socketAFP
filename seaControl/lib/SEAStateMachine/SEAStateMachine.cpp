//
// Created by Biomech on 6/14/2021.
//

#include "SEAStateMachine.h"

State SEAStateMachine::SEAState_Setup(){
    SEAstate.setState( inSetup);
    SEAstate.setStateType(SEA);

    Serial.begin(115200);
    Serial.println("online");

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
    return SEAstate;
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
    State tempState = this->SEAstate;
    if(tempState.stateChange){
        SEAOutgoing.generalMessage(tempState.getState(), "transitioned to idle");
        tempState.stateChange = false;
    }

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
            SEAOutgoing.generalMessage(tempState.getState(), "State from incoming message");
//            SEAOutgoing.generalMessage(tempState.getState(), ("Serial available: " + checkSerAgain));
            SEAOutgoing.sendComplete(tempState.getStateTypeString(), idle, true,
                                     SEAIncoming.getState().getState(), this->yEncPos);
//            printedStop = false;
//            printedIdle = false;
            return SEAIncoming.getState();

        }
    }
    else if( tempState.getState() == placement){
        return tempState;
    }
    else{
//        State tempState = this->SEAstate;
        tempState.setState(manDrive(tempState.getState()));
        return tempState;
    }
    return tempState;
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

/*
 * instructions for axis init
 * go up until limit switch is hit.
 * the checkLimits function changes the state, not this function
 */
State SEAStateMachine::SEAState_axisInit() {
    SEAMotor.driveYMotor(initUpSpeed, true, yEncPos);
//    SEAOutgoing.generalMessage(SEAstate.getState(), "axis init");
    return this->SEAstate;
}

State SEAStateMachine::SEAState_axisInitComplete() {
    State tempState = this->SEAstate;
    SEAMotor.driveYMotor(0, false, yEncPos);
    delay(500);
    SEAMotor.driveYMotor(touchHardStop, true, yEncPos);
    delay(500);
    SEAMotor.driveYMotor(0, false, yEncPos);
    delay(100);
    seaParams.firstInit = true;
    seaParams.yEnc.write(0);
    delay(100);
    SEAMotor.driveYMotor(manDownSpeed, true, yEncPos);
    this->yEncPos = seaParams.yEnc.read();
    while(this->yEncPos > -2000){
        this->yEncPos = seaParams.yEnc.read();
        // do nothing, keep driving;
    }
    SEAMotor.driveYMotor(0, false, yEncPos);
    delay(1000);
    //SEAstate = start;


    SEAOutgoing.generalMessage(tempState.getState(), "axis init complete");

    tempState.setState(idle);

    return tempState;
}

State SEAStateMachine::SEAState_GoToPos(unsigned long loopStartTime) {
    State tempState = this->SEAstate;

    yEncPos = seaParams.yEnc.read();
    tempState.setState( SEAMotor.pdControl(tempState.getGlobalDest(), this->yEncPos, loopStartTime, manDownSpeed, tempState.getState()));
//            unsigned long stateTime = millis() - SEAstate.getStateStartTime();
    unsigned long stateTime = tempState.getStateTime();
//    SEAOutgoing.generalMessage(tempState.getState(), String(yEncPos), "current encoder position");
//    SEAOutgoing.generalMessage(tempState.getState(), String(stateTime), "state time");
//    SEAOutgoing.generalMessage(tempState.getState(), String(tempState.getGlobalDest()), "destination");
    /*
     * go to position timeout
     * TODO make this based on unchanging encoder value.
     */
    if(stateTime > 1000 ){
        SEAOutgoing.sendComplete(tempState.getStateTypeString(), tempState.getState(), true, stopped,
                                 yEncPos);
        tempState.setState( stopped);
        //seaParams.stateStart = millis();
//        SEAOutgoing.generalMessage(tempState.getState(), "go to pos over");

    }

    return tempState;
}

void SEAStateMachine::runSEAStateMachine() {

    // check if the limit switch is hit
   this->SEAstate.setState( checkLimits(SEAstate.getState()));
   this->SEAstate.setState( startStop(SEAstate.getState()));

//    long loopStartTime = millis();

    // TODO: turn this into function in SEAStateMachine
    //read encoders
    this->yEncPos = seaParams.yEnc.read();
    this->SEAEncPos = seaParams.SEAenc.read();


    if(seaParams.yEncOld != this->yEncPos){
        seaParams.yEncOld = this->yEncPos;
    }

    if(seaParams.SEAencOld != this->SEAEncPos){
        seaParams.SEAencOld = this->SEAEncPos;
//        SEAOutgoing.generalMessage(this->SEAstate.getState(), String(SEAEncPos), "SEA position");

    }

    unsigned long loopStartTime = millis();
    switch (this->SEAstate.getState()){

        case stopped:
            this->SEAstate = this->SEAState_stopped();

            break;

        case idle:
            this->SEAstate = this->SEAState_idle();

            break;

            //move up, manual
        case manUp:

            this->SEAstate = this->SEAState_manUp();

            break;

            // move down, manual
        case manDown:

            this->SEAstate = this->SEAState_manDown();

            break;

        case start:

            this->SEAstate = this->SEAState_start();


            break;

        case goToStart:
            break;

            // this will run until the stage hits the limit switch.
        case axisInit:
            this->SEAstate = this->SEAState_axisInit();


            break;

        case axisInitComplete:
            this->SEAstate = this->SEAState_axisInitComplete();

            break;

        case GoToPos:

            this->SEAstate = this->SEAState_GoToPos(loopStartTime);

            break;

        case prePlacement:
            this->SEAstate = this->SEAState_prePlacement();
            break;

        case placement:
            this->SEAstate = this->SEAState_placement(loopStartTime);
            break;

    }

}

/*
 * Pre-placement
 * drive down until making contact with surface
 * with contact create sendComplete message then switch to placement state.
 */
State SEAStateMachine::SEAState_prePlacement() {
    State tempState = this->SEAstate;

    SEAMotor.driveYMotor(manDownSpeed, true, yEncPos);
    this->SEAEncPos = seaParams.SEAenc.read();

    if(this->SEAEncPos > -10){
        tempState.setState(placement);
        SEAOutgoing.sendComplete(tempState.getStateTypeString(),tempState.getState(), false,
                                 placement, this->SEAEncPos);
    }
//
//        SEAMotor.driveYMotor(0, false, yEncPos);
//        SEAOutgoing.sendComplete(tempState.getStateTypeString(),tempState.getState(), true,
//                                 placement, SEAEncPos);
//        tempState.initStartTime();
//    }
    Serial.println(tempState.getState());
    return tempState;
}

State SEAStateMachine::SEAState_placement(unsigned long loopStartTime) {
    State tempState = this->SEAstate;

    SEAEncPos = seaParams.SEAenc.read();
    tempState.setState( SEAMotor.pdControl(-1500, this->SEAEncPos, loopStartTime,
                                           manDownSpeed, tempState.getState()));
//    SEAOutgoing.generalMessage(tempState.getState(),String(SEAEncPos), "sea encoder");
    tempState = SEAState_idle();

//    if(tempState.getStateTime() > 1000){
//        tempState.setState(idle);
//        SEAOutgoing.sendComplete(tempState.getStateTypeString(), tempState.getState(), true, idle,
//                                 SEAEncPos);
//    }

    return tempState;
}

