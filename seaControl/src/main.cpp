#include <Arduino.h>
//#include <../lib/Encoder/Encoder.h>
#include <SeaControl.H>
#include <../lib/State/State.h>
#include "../lib/DualMC33926MotorShield/DualMC33926MotorShield.h"
#include "../lib/ButtonBehavior/ButtonBehavior.h"
#include "../lib/ActuatorControl/ActuatorControl.h"
#include "../lib/serialComs/SerialComs.h"
#include "../lib/QuadEncoder/QuadEncoder.h"

DualMC33926MotorShield md;
ActuatorControl SEAMotor;
SerialComs incoming;
SerialComs outgoing;

//enum state SEAstate.setState(stopped;

State SEAstate;
enum state tempState;


//void stopIfFault()
//{
//    if (md.getFault())
//    {
//        Serial.println("fault");
//        while(1);
//    }
//}
//////////////////////////////
// setup
// initalizing all the pins and their modes

void setup()
{

    SEAstate.setState( inSetup);
    Serial.begin(115200);

    yEnc.setInitConfig();
    yEnc.init();
    yEnc.write(0);

    SEAenc.setInitConfig();
    SEAenc.EncConfig.revolutionCountCondition = DISABLE;
    SEAenc.EncConfig.enableModuloCountMode = DISABLE;
    SEAenc.init();
    SEAenc.write(0);

    //SEA Motor setup
    pinMode(yPWM, OUTPUT);
    pinMode(yEnable, OUTPUT);
    pinMode(yDirection, OUTPUT);

    //SEA limit switch setup;
    pinMode(yUpLimitButtonPin, INPUT_PULLUP);
    pinMode(yDownLimitButtonPin, INPUT_PULLUP);

    //UI buttons
    pinMode(startStopButton, INPUT_PULLUP);
    pinMode(manUpButton, INPUT_PULLUP);
    pinMode(manDownButton, INPUT_PULLUP);

    //encoder index
    pinMode(yEncIndex, INPUT);
    pinMode(SEAEncIndex, INPUT);

    SEAstate.setState( stopped);

    SEAMotor.motorControlInit(yPWM, yEnable, yDirection);
    outgoing.generalMessage(SEAstate.getState(), "end setup");

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

    //read encoders
    long yEncPos = yEnc.read();
    long SEAEncPos = SEAenc.read();
    if(yEncOld != yEncPos){
        yEncOld = yEncPos;
    }

    if(SEAencOld != SEAEncPos){
        SEAencOld = SEAEncPos;
        outgoing.generalMessage(SEAstate.getState(), String(SEAEncPos));

    }

    // behavior when the machine is in a stopped state
    switch (SEAstate.getState()){

        case stopped:
            SEAMotor.driveYMotor(0, false, yEncPos);

            if(!printedStop){
                //outgoing.generalMessage(SEAstate, "stopped");
                //Serial.println("stopped");
                printedStop = true;
            }
            SEAstate.setState( idle);

            break;

        case idle:
            // check the manual drive buttons

            SEAstate.setState( manDrive(SEAstate.getState()) );

            if(!printedStop){
                outgoing.generalMessage(SEAstate.getState(), "stopped");
//                Serial.println("stopped");
                printedStop = true;
            }

            if(Serial.available()){
                incoming.checkSerial(); // this includes a loop, should block until a new line
                if(incoming.checkComplete()) {
                    incoming.readIncomingJson();
                    SEAstate = incoming.getState();
                    String checkSerAgain = String(Serial.available());
                    outgoing.generalMessage(SEAstate.getState(), "State from incoming message");
                    outgoing.generalMessage(SEAstate.getState(), ("Serial available: " + checkSerAgain));

                    printedStop = false;
                    printedIdle = false;
                }
            }

            break;

            //move up, manual
        case manUp:
            // drive motor: slowUp, enabled, up
            SEAMotor.driveYMotor(manUpSpeed, true, yEncPos);
            SEAstate.setState(manDrive(SEAstate.getState()));
//            outgoing.generalMessage(SEAstate.getState(), String(yEncPos));
            break;

            // move down, manual
        case manDown:
            // drive motor, slowDown, enabled, down
            SEAMotor.driveYMotor(manDownSpeed, true, yEncPos);
            SEAstate.setState( manDrive(SEAstate.getState()));
//            outgoing.generalMessage(SEAstate.getState(), String(yEncPos));
            break;

        case start:
            // run init if first init hasnt happened.
            if(!firstInit){
                SEAstate.setState( axisInit);
            }
                // reinitialize encoder
            else if( yEncPos < encoderReInitThreshold) {
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
            firstInit = true;
            yEnc.write(0);
            delay(100);
            SEAMotor.driveYMotor(manDownSpeed, true, yEncPos);
            yEncPos = yEnc.read();
            while(yEncPos > -2000){
                yEncPos = yEnc.read();
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
            yEncPos = yEnc.read();
            SEAstate.setState( SEAMotor.pdControl(SEAstate.getGlobalDest(), yEncPos, loopStartTime, manDownSpeed, SEAstate.getState()));
            long stateTime = millis() - SEAstate.getStateStartTime();
            if(stateTime > 1000 ){
                SEAstate.setState( stopped);
                stateStart = millis();
                outgoing.generalMessage(SEAstate.getState(), "go to pos over");
            }
            break;

        default:
            SEAstate.setState(stopped);
            break;

    }


}



