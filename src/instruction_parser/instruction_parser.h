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
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "config/config.h"
#include "validator/validator.h"

enum InstructionState : uint8_t
{
    IDLE,
    GET_INST,
    PARSE_GOAL,
    PARSE_WAIT,
    VALIDATE,
    VALIDATE_HOLD,
    KEEP_INST,
    INVALID
};

enum InstructionErrCodes : int8_t
{
    SUCCESS = 0,

    EOF_REACHED = 1,

    FILE_NOT_OPEN = -1,
    GETLINE_FALED = -2,
    NOT_ENOUGH_MEMBERS = -3,

}

class Instruction
{
public:
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
    std::vector<Instruction *> get_instruction_set();
    int8_t set_get_new_inst_set(bool get_new_inst_set);

    InstructionParser();
    void tick();

private:
    InstructionState current_state;

    std::ifstream infile;
    std::string instr_line;
    Instruction *curr_instruction;
    bool is_valid_instruction;
    std::vector<Instruction*> instruction_set;
    bool GET_NEW_INST_SET;
    Validator validator;

    int8_t errcode;

    int8_t read_instruction_from_infile();
    int16_t get_first_token();
    int8_t parse_goal_instruction();
    int8_t parse_wait_instruction();
    int8_t add_instruction_to_set();
};

#endif