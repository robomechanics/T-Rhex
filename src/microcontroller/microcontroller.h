/**
 * Interface file for the Microcontroller
 * 
 * Created by Vivaan Bahl, vrbahl
 * 
 */

#ifndef MICRO_H
#define MICRO_H

#include <vector>

#include "dynamixel_interface/dynamixel_interface.h"
#include "instruction_parser/instruction_parser.h"
#include "config/config.h"

enum class MicrocontrollerState : uint8_t
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
    uint16_t* get_leg_data();
    Instruction* get_curr_inst();
    void set_curr_ins_finished(bool is_finished);
    void set_leg_data(uint16_t leg_data[NUM_LEGS]);
    bool get_send_instr();

    Microcontroller(std::vector<Instruction*> instruction_set);
    void tick();

private:
    MicrocontrollerState current_state;

    uint16_t insctr;
    Instruction *initcmd;
    bool reinitialize;
    bool shutdown;
    uint16_t leg_data[NUM_LEGS];
    Instruction *curr_ins;
    bool curr_ins_finished;
    std::vector<Instruction *> inst_set;
    uint16_t N;
    bool send_new_ins;
    Instruction *shutdown_cmd;

    int errcode;

};

#endif