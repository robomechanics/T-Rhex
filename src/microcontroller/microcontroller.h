/**
 * Interface file for the Microcontroller
 * 
 * Created by Vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 * 
 */

#ifndef MICRO_H
#define MICRO_H

enum MicrocontrollerState
{
    INIT,
    CMD,
    SHUTDOWN,
    IDLE,
    SWITCH_INST,
    WAIT_FOR_COMP,
    INVALID
};

class Microcontroller
{
private:
    MicrocontrollerState current_state;
};

#endif