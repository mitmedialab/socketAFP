//
// Created by Biomech on 2/2/2021.
//

// Sends a pulse on the STEP pin to tell the driver to take one step, and also
//delays to control the speed of the motor.
#include "HorizontalStepperControl.h"
#include <Arduino.h>


// Sends a pulse on the STEP pin to tell the driver to take one step, and also
//delays to control the speed of the motor.
void HorizontalStepperControl::step(uint8_t stepperPin)
{
    // The STEP minimum high pulse width is 1.9 microseconds.
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(3);
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(3);
}

// Writes a high or low value to the direction pin to specify what direction to
// turn the motor.
void HorizontalStepperControl::setDirection(bool dir, uint8_t dirPin)
{
    // The STEP pin must not change for at least 200 nanoseconds before and after
    // changing the DIR pin.
    delayMicroseconds(1);
    digitalWrite(dirPin, dir);
    delayMicroseconds(1);
}