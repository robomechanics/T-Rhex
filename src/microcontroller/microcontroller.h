/**
 * Interface file for the Microcontroller
 * 
 * Created by Vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 * 
 */

#ifndef MICRO_H
#define MICRO_H

#include <vector>

#include "dynamixel_interface/dynamixel_interface.h"
#include "instruction_parser/instruction_parser.h"
#include "config/config.h"

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
public:
    int8_t set_reinit_cmd(bool reinitialize);
    int8_t set_shutdown_cmd(bool shutdown);

private:
    MicrocontrollerState current_state;

    bool on_led;
    DynamixelInterface dynlib;
    uint16_t insctr;
    Instruction initcmd;
    bool reinitialize;
    bool shutdown;
    uint16_t leg_data[NUM_DYNAMIXELS];
    Instruction curr_ins;
    bool curr_ins_finished;
    std::vector<Instruction> inst_set;
    uint16_t N;
    bool send_new_ins;
    Instruction idle_cmd;
    std::vector<Instruction> new_inst_set;
    Instruction shutdown_cmd;
};

#endif