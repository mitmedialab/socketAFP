#include <Arduino.h>
//#include <../lib/Encoder/Encoder.h>
//#include <SeaControl.H>
#include <../lib/State/State.h>
#include "../lib/DualMC33926MotorShield/DualMC33926MotorShield.h"
#include "../lib/ButtonBehavior/ButtonBehavior.h"
#include "../lib/ActuatorControl/ActuatorControl.h"
#include "../lib/serialComs/SerialComs.h"
#include "../lib/QuadEncoder/QuadEncoder.h"
#include "../lib/SEAParams/SEAParams.h"
#include "../lib/SEAStateMachine/SEAStateMachine.h"

DualMC33926MotorShield md;
ActuatorControl SEAMotor;
SerialComs incoming;
SerialComs outgoing;
SEAParams seaParams;
//enum state SEAstate.setState(stopped;

State SEAstate;
SEAStateMachine SEA_StateMachine;
//enum state tempState;


//////////////////////////////
// setup
// initalizing all the pins and their modes

void setup()
{


    SEA_StateMachine.SEAState_Setup();
    SEAstate = SEA_StateMachine.SEAstate;
    SEAMotor = SEA_StateMachine.SEAMotor;


}

//////////////////////////////////////////////////////////////////
//  Main loop
// everything happens here
//////////////////////////////////////////////////////////////////
bool printedIdle = false;
bool printedStop = false;

void loop() {

    // check if the limit switch is hit
    SEAstate.setState( checkLimits(SEAstate.getState()));
    SEAstate.setState( startStop(SEAstate.getState()));

    long loopStartTime = millis();

    // TODO: turn this into function in SEAStateMachine
    //read encoders
    long yEncPos = seaParams.yEnc.read();
    long SEAEncPos = seaParams.SEAenc.read();
    SEA_StateMachine.yEncPos = yEncPos;
    SEA_StateMachine.SEAEncPos = SEAEncPos;

    if(seaParams.yEncOld != yEncPos){
        seaParams.yEncOld = yEncPos;
    }

    if(seaParams.SEAencOld != SEAEncPos){
        seaParams.SEAencOld = SEAEncPos;
        outgoing.generalMessage(SEAstate.getState(), String(SEAEncPos));

    }




    // behavior when the machine is in a stopped state
    switch (SEAstate.getState()){

        case stopped:
            SEAstate = SEA_StateMachine.SEAState_stopped();
            SEA_StateMachine.SEAstate = SEAstate; // temporary
            break;

        case idle:
            SEAstate = SEA_StateMachine.SEAState_idle();
            SEA_StateMachine.SEAstate = SEAstate; // temporary
            break;

            //move up, manual
        case manUp:

            SEAstate = SEA_StateMachine.SEAState_manUp();
            SEA_StateMachine.SEAstate = SEAstate;
            break;

            // move down, manual
        case manDown:

            SEAstate = SEA_StateMachine.SEAState_manDown();
            SEA_StateMachine.SEAstate = SEAstate;
            break;

        case start:
            // run init if first init hasnt happened.
            if(!seaParams.firstInit){
                SEAstate.setState( axisInit);
            }
                // reinitialize encoder
            else if( yEncPos < seaParams.encoderReInitThreshold) {
                SEAstate.setState( axisInit);
            }
                // move onto next state
            else {
                SEAstate.setState( GoToPos);
            }
            break;
        case goToStart:
            break;

            // this will run until the stage hits the limit switch.
        case axisInit:
            SEAMotor.driveYMotor(initUpSpeed, true, yEncPos);
            outgoing.generalMessage(SEAstate.getState(), "axis init");
            break;

            // once a limit switch is hit
            // motor will stop, then restart and move slowly up against the hardstop
            // will resend system to start state.
        case axisInitComplete:
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
            yEncPos = seaParams.yEnc.read();
            while(yEncPos > -2000){
                yEncPos = seaParams.yEnc.read();
                // do nothing, keep driving;
            }
            SEAMotor.driveYMotor(0, false, yEncPos);
            delay(1000);
            //SEAstate = start;
            SEAstate.setState( idle);

            outgoing.generalMessage(SEAstate.getState(), "axis init complete");
            printedStop = false;
            printedIdle = false;
            break;

        case GoToPos:

            yEncPos = seaParams.yEnc.read();
            SEAstate.setState( SEAMotor.pdControl(SEAstate.getGlobalDest(), yEncPos, loopStartTime, manDownSpeed, SEAstate.getState()));
//            unsigned long stateTime = millis() - SEAstate.getStateStartTime();
            unsigned long stateTime = SEAstate.getStateTime();
            outgoing.generalMessage(SEAstate.getState(), String(yEncPos));
            outgoing.generalMessage(SEAstate.getState(), String(stateTime));
            if(stateTime > 1000 ){
                SEAstate.setState( stopped);
                //seaParams.stateStart = millis();
                outgoing.generalMessage(SEAstate.getState(), "go to pos over");
            }
            break;

//        default:
//            SEAstate.setState(stopped);
//            break;

    }


}



