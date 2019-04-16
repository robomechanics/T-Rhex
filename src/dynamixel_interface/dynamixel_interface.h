/**
 * Interface file for the dynamixel interface
 * 
 * Created by Vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 */

#ifndef DYN_INTF_H
#define DYN_INTF_H

enum DynInterfaceState
{
    INIT,
    IDLE,
    SET_BAUD,
    SEND_INSTR,
    READ_DATA,
    STOP_MOTORS,
    FINISH,
    SHUTDOWN,
    INVALID
};

class DynamixelInterface
{
private:
    DynInterfaceState current_state;
};

#endif