//
// Created by Biomech on 6/16/2021.
//

#ifndef SEACONTROL_MULTIDOFSTATEMACHINE_H
#define SEACONTROL_MULTIDOFSTATEMACHINE_H

#include <../State/State.h>
//#include "../ButtonBehavior/ButtonBehavior.h"
#include "../ActuatorControl/ActuatorControl.h"
#include "../serialComs/SerialComs.h"
//#include "../QuadEncoder/QuadEncoder.h"
//#include "../SEAParams/SEAParams.h"

class multiDofStateMachine {

    State MultiDof_stopped();
    State MultiDof_idle();
    State ZTranslate_gotToPos();
    State ZTranslate_stopped();
    State ZTranslate_idle();
    State ZRotate_goToPos();
    state ZRotate_stopped();
    State ZRotate_idle();
    State AlphaRotate_goToPos();
    State AlphaRotate_stopped();
    State SecondHeaterHigh();
    State SecondHeaterLow();

public:
    State MultiDof_Setup();
    void runMultiDofState();
    void runZtranslateStateMachine();
    void runZrotateStateMachine();
    void runAlphaRotateStateMachine();

    State MultiDofState;
    State ZHorizontalState;
    State ZRotationState;
    State AlphaState;

private:
    SerialComs MultiDofIncoming;
    SerialComs MultiDofOutgoing;
};


#endif //SEACONTROL_MULTIDOFSTATEMACHINE_H
