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
    enum stateType incomingTemp = (incoming["stateType"][0]);
    if(incomingTemp == SEA ){
        this->messageState = this->generateState();
    }
    else{
        Serial.println("generate MultiState");
        this->messageMultiState = this->generateMultiState();
    }
    this->incoming.clear();
}

void SerialComs::sendJson() {
    this->doc["msgID"] = this->msgID;
    this->msgID++;
    serializeJson(this->doc, Serial);
    this->doc.clear();

}

void SerialComs::generalMessage(enum state currentState, String message, String message2) {
    this->doc["state"] = currentState;
    this->doc["message"] = message;
    this->doc["extra info"] = message2;
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

State SerialComs::generateState() {
    enum state tempState = incoming["state"][0];
    enum stateType tempStateType = incoming["stateType"][0];
    long position = incoming["go_y_position"][0];
    float pGain = incoming["pGain"][0];
    float iGain = incoming["iGain"][0];
    float dGain = incoming["dGain"][0];
    boolean move = incoming["move_y"][0];
    State tempMsgState;
    tempMsgState.setState(tempState);
    tempMsgState.setStateType(tempStateType);
    tempMsgState.setMove(move);
    tempMsgState.goToGlobalPos(position, pGain, iGain, dGain, 0);
    tempMsgState.initStartTime();

    return tempMsgState;
}


State SerialComs::getState() {
    return this-> messageState;
}

/*
 * this function generates the states for the multidof controller
 * if the state is
 */
MultiState SerialComs::generateMultiState() {

    /*
     * naming here isnt great...
     * booleans are if the dof is active
     * ints are the values they are set to go to.
     */
    boolean moveZ = incoming["move_z"][0];
    boolean rotZ = incoming["rotate_z"][0];
    boolean rotA = incoming["rotate_alpha"][0];
    long zPos = incoming["go_z_position"][0];
    long zRote = incoming["go_z_rotation"][0];
    long aRote = incoming["go_alpha_rotation"][0];

    /*
     * create the multiState
     * build the substates.
     */
    MultiState tempMultiState;

    tempMultiState.setZTState(tempMultiState.buildState(zPos, moveZ, zHorizontal));
    tempMultiState.setZRState(tempMultiState.buildState(zRote, rotZ, zRotate));
    tempMultiState.setAState(tempMultiState.buildState(aRote, rotA, alphaRotate));

    return tempMultiState;

}

MultiState SerialComs::getMultiState() {
    return this->messageMultiState;
}






