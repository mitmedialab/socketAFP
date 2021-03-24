//
// Created by Biomech on 2/9/2021.
//

#include "SerialComs.h"


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

//void SerialComs::motorState(enum state currentState, long encoderVal, long error) {
//    doc["state"] = currentState;
//    doc["encoder"] = encoderVal;
//    doc["error"]  = error;
//    sendJson();
//}

void SerialComs::sendJson() {
    serializeJson(doc, Serial);
    doc.clear();
}

void SerialComs::generalMessage(enum state currentState, String message) {
    doc["state"] = currentState;
    doc["message"] = message;
    sendJson();
}

void SerialComs::motorState(enum state currentState, long encoderVal, long error, int setPWM, int pTerm, double pError,
                            int dTerm, double dError, int iTerm, double iError) {
    doc["state"] = currentState;
    doc["encoder"] = encoderVal;
    doc["error"]  = error;
    doc["setPWM"] = setPWM;
    doc["pTerm"] = pTerm;
    doc["pError"] = pError;
    doc["dTerm"] = dTerm;
    doc["dError"] = dError;
    doc["iTerm"] = iTerm;
    doc["iEror"] = iError;
    sendJson();

}



