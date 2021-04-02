//
// Created by Biomech on 2/9/2021.
//

#ifndef SEACONTROL_SERIALCOMS_H
#define SEACONTROL_SERIALCOMS_H
#include <Arduino.h>
#include "../ArduinoJson/ArduinoJson.h"
#include "../State/State.h"



class SerialComs {

public:
    void checkSerial();
    bool checkComplete();
    String getMessage();
//    json incomingmsg;

    void readIncomingJson();
    void motorState(enum state currentState, long encoderVal, long error, int setPWM, int pTerm, double pError, int dTerm,
            double dError, int iTerm, double iError);
    void generalMessage(enum state currentState, String message);
    void sendJson();
    State getState();

private:
    StaticJsonDocument<200> doc;
    StaticJsonDocument<2048> incoming;

    void generateState();
    State messageState;
    bool stringComplete = false;
    String newMsg = "";
    uint32_t msgID = 0;


};


#endif //SEACONTROL_SERIALCOMS_H
