#include <Arduino.h>
#include <../lib/Encoder/Encoder.h>
#include <SeaControl.H>
#include <../lib/State/State.h>
#include "../lib/SEAMotorControl/SEAMotorControl.h"
#include "../lib/DualMC33926MotorShield/DualMC33926MotorShield.h"
#include "../lib/ButtonBehavior/ButtonBehavior.h"
DualMC33926MotorShield md;
SEAMotorControl openControl;

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
}

//////////////////////////////////////////////////////////////////
//  Main loop
// everything happens here
//////////////////////////////////////////////////////////////////
void loop() {

    // check if the limit switch is hit
    SEAstate = checkLimits(SEAstate);
    SEAstate = startStop(SEAstate);


    //read encoders
    long yEncPos = yEnc.read();
    if(yEncOld != yEncPos){
        //Serial.println(yEncPos);
        yEncOld = yEncPos;
    }


    // behavior when the machine is in a stopped state
    switch (SEAstate){
        case stopped:
            openControl.driveYMotor(0, false, yEncPos);
            // check the manual drive buttons
            //SEAstate = manDrive(SEAstate);
            SEAstate = idle;

            break;

        case idle:
            // check the manual drive buttons
            SEAstate = manDrive(SEAstate);
            break;

            //move up, manual
        case manUp:
            // drive motor: slowUp, enabled, up
            openControl.driveYMotor(manUpSpeed, true, yEncPos);
            SEAstate = manDrive(SEAstate);
            break;

            // move down, manual
        case manDown:
            // drive motor, slowDown, enabled, down
            openControl.driveYMotor(manDownSpeed, true, yEncPos);
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
                SEAstate = idle;
            }
            break;
        case goToStart:
            break;

            // this will run until the stage hits the limit switch.
        case axisInit:
            openControl.driveYMotor(initUpSpeed, true, yEncPos);
            break;

            // once a limit switch is hit
            // motor will stop, then restart and move slowly up against the hardstop
            // will resend system to start state.
        case axisInitComplete:
            openControl.driveYMotor(0, false, yEncPos);
            delay(500);
            openControl.driveYMotor(touchHardStop, true, yEncPos);
            delay(500);
            openControl.driveYMotor(0, false, yEncPos);
            delay(100);
            firstInit = true;
            yEnc.write(0);
            SEAstate = start;
            break;

        case down:
            break;
        case up:
            break;


    }
}