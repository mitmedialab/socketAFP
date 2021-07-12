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
    ActuatorControl SEAMotor; // shouldn't have used SEA in the name for this... motor control object for when not using the Series elastic spring
    ActuatorControl CompactionControl; // motor control object for when using the Series elastic spring
    State SEAState_Setup();
    State SEAState_stopped();
    State SEAState_idle();
    State SEAState_manUp();
    State SEAState_manDown();
    State SEAState_start();
    State SEAState_axisInit();
    State SEAState_axisInitComplete();
    State SEAState_GoToPos(unsigned long loopStartTime);
    State SEAState_prePlacement();
    State SEAState_placement(unsigned long loopStartTime);
    void runSEAStateMachine();

    long yEncPos;
    long SEAEncPos;
};



#endif //SEACONTROL_SEASTATEMACHINE_H
