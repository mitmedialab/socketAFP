//
// Created by Biomech on 2/9/2021.
//

#ifndef SEACONTROL_SERIALCOMS_H
#define SEACONTROL_SERIALCOMS_H
#include <Arduino.h>
#include "../ArduinoJson/ArduinoJson.h"
#include "../State/State.h"
#include "../MultiState/MultiState.h"


class SerialComs {

public:
    void checkSerial();
    bool checkComplete();
    String getMessage();
//    json incomingmsg;

    void readIncomingJson();
    void motorState(enum state currentState, long encoderVal, long error, int setPWM, int pTerm, double pError, int dTerm,
            double dError, int iTerm, double iError);
    void generalMessage(enum state currentState, String message, String message2 = "");
    void sendJson();
    State getState();
    MultiState getMultiState();
private:
    StaticJsonDocument<200> doc;
    StaticJsonDocument<2048> incoming;

    State generateState(); // this is for the SEA controller
    MultiState generateMultiState(); // this is for the multiDof controller.
    State messageState;
    MultiState messageMultiState;
    bool stringComplete = false;
    String newMsg = "";
    uint32_t msgID = 0;


};


#endif //SEACONTROL_SERIALCOMS_H
