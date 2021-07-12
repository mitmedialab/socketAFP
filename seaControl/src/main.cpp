//#include "../lib/SEAStateMachine/SEAStateMachine.h"
//
//SEAStateMachine SEA_StateMachine;
////enum state tempState;
//
//
////////////////////////////////
//// setup
//// initalizing all the pins and their modes
//
//void setup()
//{
//    SEA_StateMachine.SEAState_Setup();
//
//}
//
//////////////////////////////////////////////////////////////////////
//////  Main loop
////// everything happens here
//////////////////////////////////////////////////////////////////////
//
//void loop() {
//
//    SEA_StateMachine.runSEAStateMachine();
//
//}


#include "../lib/multiDofStateMachine/multiDofStateMachine.h"

multiDofStateMachine MultiDof_StateMachine;
//enum state tempState;


//////////////////////////////
// setup
// initalizing all the pins and their modes

void setup()
{
    MultiDof_StateMachine.MultiDof_Setup();
}

//////////////////////////////////////////////////////////////////
//  Main loop
// everything happens here
//////////////////////////////////////////////////////////////////

void loop() {

    MultiDof_StateMachine.runMultiDofState();

}
