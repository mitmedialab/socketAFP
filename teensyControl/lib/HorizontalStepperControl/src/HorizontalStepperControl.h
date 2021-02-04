//
// Created by Biomech on 2/2/2021.
//

#ifndef TEENSYCONTROL_HORIZONTALSTEPPERCONTROL_H
#define TEENSYCONTROL_HORIZONTALSTEPPERCONTROL_H

#endif //TEENSYCONTROL_HORIZONTALSTEPPERCONTROL_H
#include <Arduino.h>


class HorizontalStepperControl
{
public:
    void step(uint8_t stepperPin);
    void setDirection(bool dir, uint8_t dirPin);
};

