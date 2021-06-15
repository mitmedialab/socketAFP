//
// Created by Biomech on 6/14/2021.
//

#ifndef SEACONTROL_SEASTATEMACHINE_H
#define SEACONTROL_SEASTATEMACHINE_H

#include <Arduino.h>

#include <../State/State.h>
#include "../ButtonBehavior/ButtonBehavior.h"
#include "../ActuatorControl/ActuatorControl.h"
#include "../serialComs/SerialComs.h"
#include "../QuadEncoder/QuadEncoder.h"
#include "../SEAParams/SEAParams.h"

class SEAStateMachine {
public:
    SerialComs SEAIncoming;
    SerialComs SEAOutgoing;
    SEAParams seaParams;
    State SEAstate;
    ActuatorControl SEAMotor;

    void SEASetup();
};



#endif //SEACONTROL_SEASTATEMACHINE_H
