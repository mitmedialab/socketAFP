//
// Created by Biomech on 6/14/2021.
//

#include "SEAStateMachine.h"

void SEAStateMachine::SEASetup() {
    SEAstate.setState( inSetup);
    Serial.begin(115200);

    seaParams.yEnc.setInitConfig();
    seaParams.yEnc.init();
    seaParams.yEnc.write(0);

    seaParams.SEAenc.setInitConfig();
    seaParams.SEAenc.EncConfig.revolutionCountCondition = DISABLE;
    seaParams.SEAenc.EncConfig.enableModuloCountMode = DISABLE;
    seaParams.SEAenc.init();
    seaParams.SEAenc.write(0);

    //SEA Motor setup
    pinMode(seaParams.yPWM, OUTPUT);
    pinMode(seaParams.yEnable, OUTPUT);
    pinMode(seaParams.yDirection, OUTPUT);

    //SEA limit switch setup;
    pinMode(yUpLimitButtonPin, INPUT_PULLUP);
    pinMode(yDownLimitButtonPin, INPUT_PULLUP);

    //UI buttons
    pinMode(startStopButton, INPUT_PULLUP);
    pinMode(manUpButton, INPUT_PULLUP);
    pinMode(manDownButton, INPUT_PULLUP);

    //encoder index
    pinMode(seaParams.yEncIndex, INPUT);
    pinMode(seaParams.SEAEncIndex, INPUT);

    SEAstate.setState( stopped);

    SEAMotor.motorControlInit(seaParams.yPWM, seaParams.yEnable, seaParams.yDirection);
    SEAOutgoing.generalMessage(SEAstate.getState(), "end setup");
}
