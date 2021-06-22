//
// Created by Biomech on 6/22/2021.
//

#ifndef SEACONTROL_MULTIDOFPARAM_H
#define SEACONTROL_MULTIDOFPARAM_H

#include "../tic-arduino/Tic.h"
#include "Arduino.h"
class MultiDofParam {
public:
    TicI2C tic1 = TicI2C(14);
    TicI2C tic2 = TicI2C(15);


};


#endif //SEACONTROL_MULTIDOFPARAM_H
