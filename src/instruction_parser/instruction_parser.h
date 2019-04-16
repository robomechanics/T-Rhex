/**
 * 
 * interface file for the instruction parser
 * 
 * Created by vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 */

#ifndef INSTR_PARSER_H
#define INSTR_PARSER_H

#include <vector>
#include <string>
#include <cstdint>

#include "config/config.h"
#include "validator/validator.h"

enum InstructionState
{
    IDLE,
    GET_INST,
    PARSE_GOAL,
    PARSE_WAIT,
    VALIDATE,
    KEEP_INST,
    INVALID
};

class Instruction
{
    uint16_t goal_positions[NUM_DYNAMIXELS];
    uint16_t goal_velocities[NUM_DYNAMIXELS];
    bool is_wait_instr;
    uint16_t wait_time_ms;
};

class InstructionParser
{
public:
    int8_t set_infile(std::string infile);
    int8_t set_is_valid_instruction(bool is_valid_instruction);
    std::vector<Instruction> get_instruction_set();
    int8_t get_new_inst_set(bool get_new_inst_set);

private:
    InstructionState current_state;

    std::string infile;
    std::string instr_line;
    Instruction curr_instruction;
    bool is_valid_instruction;
    std::vector<Instruction> instruction_set;
    bool GET_NEW_INST_SET;
    Validator validator;
};

#endif