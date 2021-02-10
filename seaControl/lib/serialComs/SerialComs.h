//
// Created by Biomech on 2/9/2021.
//

#ifndef SEACONTROL_SERIALCOMS_H
#define SEACONTROL_SERIALCOMS_H
#include <Arduino.h>
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class SerialComs {

public:
    void checkSerial();
    bool checkComplete();
    String getMessage();
    json incomingmsg;
private:
    bool stringComplete = false;
    String newMsg = "";
    json j;
};


#endif //SEACONTROL_SERIALCOMS_H
