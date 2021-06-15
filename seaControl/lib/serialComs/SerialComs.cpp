//
// Created by Biomech on 2/9/2021.
//

#include "SerialComs.h"


void SerialComs::checkSerial() {

    while( Serial.available() && !this->stringComplete){
        char inChar = (char)Serial.read();
        this->newMsg += inChar;

        if(inChar == '\n'){
            this->stringComplete = true;
        }
    }
}

bool SerialComs::checkComplete() {
    if(this->stringComplete){
        this->stringComplete = false;
        return true;
    }
    else {
        return false;
    }
}

// getter of most recent incoming message
// clears
String SerialComs::getMessage() {
    String output = this->newMsg;
    this->newMsg = "";
    return output;
}

void SerialComs::readIncomingJson(){
    String rawMsg = this->getMessage(); //gets the message and clears the stored message.
    Serial.print("the raw message is: ");
    Serial.println(rawMsg);
    DeserializationError error = deserializeJson(incoming, rawMsg);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    this->generateState();
    this->incoming.clear();
}

void SerialComs::sendJson() {
    this->doc["msgID"] = this->msgID;
    this->msgID++;
    serializeJson(this->doc, Serial);
    this->doc.clear();

}

void SerialComs::generalMessage(enum state currentState, String message) {
    this->doc["state"] = currentState;
    this->doc["message"] = message;
    sendJson();
}

void SerialComs::motorState(enum state currentState, long encoderVal, long error, int setPWM, int pTerm, double pError,
                            int dTerm, double dError, int iTerm, double iError) {
    this->doc["state"] = currentState;
    this->doc["encoder"] = encoderVal;
    this->doc["error"]  = error;
    this->doc["setPWM"] = setPWM;
    this->doc["pTerm"] = pTerm;
    this->doc["pError"] = pError;
    this->doc["dTerm"] = dTerm;
    this->doc["dError"] = dError;
    this->doc["iTerm"] = iTerm;
    this->doc["iEror"] = iError;
    sendJson();

}

void SerialComs::generateState() {
    enum state tempState = incoming["state"][0];
    long position = incoming["go_y_position"][0];
    float pGain = incoming["pGain"][0];
    float iGain = incoming["iGain"][0];
    float dGain = incoming["dGain"][0];
    boolean move = incoming["move_y"][0];
    this->messageState.setState(tempState);
    this->messageState.setMove(move);
    this->messageState.goToGlobalPos(position, pGain, iGain, dGain, 0);
    this->messageState.initStartTime();
}

State SerialComs::getState() {
    return this-> messageState;
}




