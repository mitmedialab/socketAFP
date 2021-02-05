//
// Created by Biomech on 2/5/2021.
//

#include "ActuatorControl.h"

void ActuatorControl::motorControlInit(uint8_t pwm, uint8_t enable, uint8_t direction) {
    pwmPin = pwm;
    enablePin = enable;
    dirPin = direction;

}

void ActuatorControl::driveYMotor(int motorSpeed, bool enable, long yEncoderPos) {
    int pwm;
    bool mDirection; // false is down, true is up

    // go up
    if(motorSpeed >= 3){
        pwm = motorSpeed;
        mDirection = true;
    }

        // go down
    else if(motorSpeed <= -3){
        pwm = abs(motorSpeed);
        mDirection = false;
    }

        // close to 0, up
    else if(motorSpeed  < 3 && motorSpeed >= 0){
        pwm =3;
        mDirection = true;
    }

        // close to 0, down
    else if(motorSpeed  > -3 && motorSpeed < 0){
        pwm =3;
        mDirection = false;
    }

    // slow down if its close to a button
    //
    if(yEncoderPos > -5000 && mDirection) {
        if(abs(motorSpeed) > abs(initUpSpeed)){
            pwm = initUpSpeed;
            mDirection = true;
        }
    }
    if(yEncoderPos < -43000 && !mDirection) {
        if(abs(motorSpeed) > abs(10)){
            pwm = 29;
            mDirection = false;

        }
    }

    //write the values to the motor
    analogWrite(pwmPin, pwm);
    digitalWrite(enablePin, enable);
    digitalWrite(dirPin, mDirection);
    //Serial.println(pwm);
}

enum state ActuatorControl::pdControl(long desiredPosition, long yEncoderPos, long currentTime, int basePWM) {
    enum state actuatorState;
    //yEncPos = yEnc.read();
    pError = double(desiredPosition - yEncoderPos);
    dError = (pError - dError) /  double(currentTime);
    int setPWM =0;

    if(true){

        int pTerm = int(pGain * pError);
        int dTerm = int(dGain * dError);
        setPWM = pTerm + dTerm;
        if(abs(pError) > 3000 && setPWM > basePWM + 5){
            setPWM = basePWM + 5;
        }

        driveYMotor(setPWM, true, yEncoderPos);
        String printString = "pwm: " + String(basePWM) + ", desired pwm: " + String(setPWM) + ", encoder: " + String(yEncoderPos) +
                ", error: " + String(pError);
        Serial.println(printString);
        actuatorState = pleaseGoToPos;
    }
    else{
        actuatorState = stopped;
    }
    return actuatorState;
}