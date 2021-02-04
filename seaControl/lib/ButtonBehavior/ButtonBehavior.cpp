//
// Created by Biomech on 2/3/2021.
//
#include "Arduino.h"
#include "ButtonBehavior.h"

// mini state machine for determining what to do when
// the manual drive buttons are pressed
//

enum state manDrive(enum state currentState) {
    bool upButton = !digitalRead(manUpButton);
    bool downButton = !digitalRead(manDownButton);
    enum state newState;
    switch(currentState){

        // if current state is stopped
        case idle:
            if(upButton){
                newState = manUp;
            }
            else if(downButton){
                newState = manDown;
            }
            else {
                newState = idle;
            }
            break;

            // if current state is down - ignore
        case down: newState = down;
            break;

            //if current state is up - ignore
        case up: newState = up;
            break;

            // if current state is manual down - continue down, or stop
        case manDown:
            if(downButton){
                newState = manDown;
            }

            else{
                newState = stopped;
            }
            break;

            // if current state is manual down - continue down, or stop
        case manUp:
            if(upButton){
                newState = manUp;
            }

            else{
                newState = stopped;
            }
            break;

//   case idle:
//    newState = idle;
//    break;
        default:
            newState = stopped;
            break;
    }

    return newState;

}

// check the limit switches
// if either is triggered return stopped
// defaults to a stop.
// behavior ignorning the button needs to be explicitly defined.
enum state checkLimits(enum state currentState){
    enum state newState = currentState;
    // read the buttons
    boolean upLimit = !digitalRead(yUpLimitButtonPin);
    boolean downLimit = !digitalRead(yDownLimitButtonPin);
    //when the up limit switch is hit
    if(upLimit){
        switch(currentState){
            case manUp:
                newState = stopped;
                break;
            case manDown:
                // ignore
                break;
            case axisInit:
                newState = axisInitComplete;
                break;
            case axisInitComplete:
                // ignore
                break;
            case start:
                // ignore
                break;
            case idle:
                // ignore
                break;
            default:
                newState = stopped;
        }
    }
    // when the down limit switch is hit
    if(downLimit){
        switch(currentState){
            case manUp:
                // ignore
                break;
            case manDown:
                newState = stopped;
                break;
            case axisInit:
                // ignore
                break;
            case axisInitComplete:
                // ignore
                break;
            case start:
                // ignore
                break;
            case idle:
                // ignore
                break;
            default:
                newState = stopped;
        }
    }
    return newState;
}

// setup the start/stop button
// first button press will set y axis in mode to run programmed behavior & lockout the manual up down buttons
// second press will set the y axis to stopped mode, motor will be disabled and manual up down will be enabled.

unsigned long buttonLockOut = 0;

enum state startStop(enum state currentState){
    // read the button
    boolean buttonPress = !digitalRead(startStopButton);
    enum state newState;

    if(buttonPress){
        // this handles debounce, max 1 button press per 300 milli seconds
        if(millis() - buttonLockOut < 300){
            return currentState;
        }

        else{
            // reset lockout counter
            buttonLockOut = millis();

            // when axis is in stopped state, switch to start state
            if(idle == currentState){
                newState = start;
            }

                // any other state go to stopped
            else{
                newState = stopped;
            }
            return newState;
        }
    }

        // if button is not pressed return whatever the current state is.
    else{
        return currentState;
    }
}
