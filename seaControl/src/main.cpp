#include <Arduino.h>
#include <../lib/Encoder/Encoder.h>
#include <SeaControl.H>
#include <../lib/State/State.h>
#include "../lib/DualMC33926MotorShield/DualMC33926MotorShield.h"
#include "../lib/ButtonBehavior/ButtonBehavior.h"
#include "../lib/ActuatorControl/ActuatorControl.h"
#include "../lib/serialComs/SerialComs.h"

DualMC33926MotorShield md;
ActuatorControl SEAMotor;
SerialComs incoming;

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
    Serial.begin(115200);
    Serial.println("Dual MC33926 Motor Shield");
    md.init();

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
    Serial.println("seriously wtf 1");
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

  // Serial.println(SEAstate);

    //read encoders
    long yEncPos = yEnc.read();
    //Serial.println(yEncPos);
    if(yEncOld != yEncPos){
        //Serial.println(yEncPos);
        yEncOld = yEncPos;
    }


    // behavior when the machine is in a stopped state
    switch (SEAstate){

        case stopped:
            SEAMotor.driveYMotor(0, false, yEncPos);
            // check the manual drive buttons
            //SEAstate = manDrive(SEAstate);

            if(!printedStop){
                Serial.println("stopped");
                printedStop = true;
            }
            SEAstate = idle;

            break;

        case idle:
            // check the manual drive buttons
            SEAstate = manDrive(SEAstate);

            if(!printedStop){
                Serial.println("stopped");
                printedStop = true;
            }

            if(Serial.available()){
                incoming.checkSerial();
                if(incoming.checkComplete()) {
                    Serial.println(incoming.getMessage());
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

            SEAMotor.driveYMotor(manDownSpeed, true, yEncPos);
            yEncPos = yEnc.read();
            while(yEncPos > -2000){
                yEncPos = yEnc.read();
                Serial.println(yEncPos);
                // do nothing, keep driving;
            }
            SEAMotor.driveYMotor(0, false, yEncPos);
            delay(1000);
            //SEAstate = start;
            SEAstate = pleaseGoToPos;
            stateStart = millis();
            Serial.println("seriously wtf 2");
            Serial.println(SEAstate);
            printedStop = false;
            printedIdle = false;
            break;

        case pleaseGoToPos:

            yEncPos = yEnc.read();
            SEAstate = SEAMotor.pdControl(-15000, yEncPos, loopStartTime, manDownSpeed, SEAstate);

            long stateTime = millis() - stateStart;
            if(stateTime > 1500 ){
                SEAstate = posTwo;
                stateStart = millis();
            }

            break;

        case posTwo:
            yEncPos = yEnc.read();
            SEAstate = SEAMotor.pdControl(-8000, yEncPos, loopStartTime, manDownSpeed, SEAstate);

            stateTime = millis() - stateStart;
            Serial.println(stateTime);
            if(stateTime > 1500 ){
                SEAstate = stopped;
            }

            break;

        default:
            SEAstate = stopped;
            break;

    }

}



