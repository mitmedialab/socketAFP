//
// Created by Biomech on 2/9/2021.
//

#include "SerialComs.h"
//#include <Arduino.h>
void SerialComs::checkSerial() {

    while( Serial.available() && !stringComplete){
        char inChar = (char)Serial.read();
        newMsg += inChar;

        if(inChar == '\n'){
            stringComplete = true;
        }
    }
}

bool SerialComs::checkComplete() {
    if(stringComplete){
        stringComplete = false;
        return true;
    }
    else {
        return false;
    }
}

String SerialComs::getMessage() {
    String output = newMsg;
    newMsg = "";
    return output;
}


