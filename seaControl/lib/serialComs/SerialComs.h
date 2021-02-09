//
// Created by Biomech on 2/9/2021.
//

#ifndef SEACONTROL_SERIALCOMS_H
#define SEACONTROL_SERIALCOMS_H
#include <Arduino.h>

class SerialComs {

public:
    void checkSerial();
    bool checkComplete();
    String getMessage();
private:
    bool stringComplete = false;
    String newMsg = "";
};


#endif //SEACONTROL_SERIALCOMS_H
