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

enum state SEAstate = stopped;



void stopIfFault()
{
    if (md.getFault())
    {
        Serial.println("fault");
        while(1);
    }
}
//////////////////////////////
// setup
// initalizing all the pins and their modes

void setup()
{
    SEAstate = inSetup;
    Serial.begin(115200);
//    outgoing.generalMessage(SEAstate, "dual motor shield init");
    //Serial.println("Dual MC33926 Motor Shield");
//    md.init();

    yEnc.setInitConfig();
    yEnc.init();
    yEnc.write(0);



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
    SEAstate = stopped;

    SEAMotor.motorControlInit(yPWM, yEnable, yDirection);
    outgoing.generalMessage(SEAstate, "end setup");

}

//////////////////////////////////////////////////////////////////
//  Main loop
// everything happens here
//////////////////////////////////////////////////////////////////
bool printedIdle = false;
bool printedStop = false;

void loop() {

    // check if the limit switch is hit
    SEAstate = checkLimits(SEAstate);
    SEAstate = startStop(SEAstate);
    unsigned long loopStartTime = millis();

    //read encoders
    long yEncPos = yEnc.read();
    if(yEncOld != yEncPos){

        yEncOld = yEncPos;
    }

    // behavior when the machine is in a stopped state
    switch (SEAstate){

        case stopped:
            SEAMotor.driveYMotor(0, false, yEncPos);
            // check the manual drive buttons
            //SEAstate = manDrive(SEAstate);

            if(!printedStop){
                //outgoing.generalMessage(SEAstate, "stopped");
                //Serial.println("stopped");
                printedStop = true;
            }
            SEAstate = idle;

            break;

        case idle:
            // check the manual drive buttons
            SEAstate = manDrive(SEAstate);

            if(!printedStop){
                outgoing.generalMessage(SEAstate, "stopped");
//                Serial.println("stopped");
                printedStop = true;
            }

            if(Serial.available()){
                incoming.checkSerial();
                if(incoming.checkComplete()) {
                    Serial.println("the message is:");
                    Serial.println(incoming.getMessage());
                    Serial.println("It worked");
                }
            }

            break;

            //move up, manual
        case manUp:
            // drive motor: slowUp, enabled, up
            SEAMotor.driveYMotor(manUpSpeed, true, yEncPos);
            SEAstate = manDrive(SEAstate);
            break;

            // move down, manual
        case manDown:
            // drive motor, slowDown, enabled, down
            SEAMotor.driveYMotor(manDownSpeed, true, yEncPos);
            SEAstate = manDrive(SEAstate);
            break;

        case start:
            // run init if first init hasnt happened.
            if(!firstInit){
                SEAstate = axisInit;
            }
                // reinitialize encoder
            else if( yEncPos < encoderReInitThreshold) {
                SEAstate = axisInit;
            }
                // move onto next state
            else {
                SEAstate = pleaseGoToPos;
            }
            break;
        case goToStart:
            break;

            // this will run until the stage hits the limit switch.
        case axisInit:
            SEAMotor.driveYMotor(initUpSpeed, true, yEncPos);
            outgoing.generalMessage(SEAstate, "axis init");
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
            SEAstate = pleaseGoToPos;
            //SEAstate = stopped;
            stateStart = millis();
            outgoing.generalMessage(SEAstate, "axis init complete");
            printedStop = false;
            printedIdle = false;
            break;

        case pleaseGoToPos:

            yEncPos = yEnc.read();
            SEAstate = SEAMotor.pdControl(-7000, yEncPos, loopStartTime, manDownSpeed, SEAstate);

            long stateTime = millis() - stateStart;
            if(stateTime > 3000 ){
                SEAstate = stopped;
                stateStart = millis();
            }

            break;

        case posTwo:
            yEncPos = yEnc.read();
            SEAstate = SEAMotor.pdControl(-5000, yEncPos, loopStartTime, manDownSpeed, SEAstate);

            stateTime = millis() - stateStart;
            Serial.println(stateTime);
            if(stateTime > 1500 ){
                SEAstate = stopped;
                printedStop = false;
                printedIdle = false;
            }

            break;

        default:
            SEAstate = stopped;
            break;

    }

}



