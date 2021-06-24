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
    else if(motorSpeed < -228){
        pwm = -228;
        mDirection = false;
    }

    else if(motorSpeed > 228){
        pwm = 228;
        mDirection = true;

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
//    outGoingMsgs.generalMessage(GoToPos, String(pwm),"pwm");
//    outGoingMsgs.generalMessage(GoToPos, String(mDirection), "direction");
}

// TODO: make init routine a function
//void ActuatorControl::initYMotor(){
//        driveYMotor(0, false, yEncPos);
//        delay(500);
//        driveYMotor(touchHardStop, true, yEncPos);
//        delay(500);
//        driveYMotor(0, false, yEncPos);
//        delay(100);
//        firstInit = true;
//        yEnc.write(0);
//
//        driveYMotor(manDownSpeed, true, yEncPos);
//        yEncPos = yEnc.read();
//        while(yEncPos > -2000){
//            yEncPos = yEnc.read();
//            Serial.println(yEncPos);
//            // do nothing, keep driving;
//        }
//        SEAMotor.driveYMotor(0, false, yEncPos);
//        delay(1000);
//
//};

enum state ActuatorControl::pdControl(long desiredPosition, long yEncoderPos, unsigned long currentTime, int basePWM, enum state currentState) {
    enum state actuatorState;
    //yEncPos = yEnc.read();
    pError = double(desiredPosition - yEncoderPos);
    dError = (pError - dError) /  double(currentTime);
    iError = (pError + iError) * currentTime;
    if(abs(iError) > 2000){
        if(iError > 0){
            iError = 2000;
        }
        else{
            iError = -2000;
        }
    }
    int setPWM =0;

    if(true){

        int pTerm = int(p_gain * pError);
        int dTerm = int(d_gain * dError);
        int iTerm = int(i_gain * iError);
        setPWM = pTerm + dTerm + iTerm;
//        setPWM = pTerm + dTerm;
//        if(setPWM > 0){
//            setPWM = setPWM + 60;
//        }
//        if(abs(pError) > 3000 && setPWM > basePWM + 5){
//            setPWM = basePWM + 5;
//        }

        driveYMotor(setPWM, true, yEncoderPos);

//        outGoingMsgs.motorState(currentState, yEncoderPos, pError, setPWM, pTerm, pError, dTerm, dError, iTerm, iError);
        actuatorState = currentState;

    }
    else{
        actuatorState = stopped;
    }
    return actuatorState;
}

void ActuatorControl::setGains(double p_gain_in, double d_gain_in, double i_gain_in) {
    p_gain = p_gain_in;
    d_gain = d_gain_in;
    i_gain = i_gain_in;

}


