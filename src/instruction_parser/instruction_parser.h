/**
 * 
 * interface file for the instruction parser
 * 
 * Created by vivaan Bahl, vrbahl
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
    std::vector<Instruction *> get_instruction_set();

    InstructionParser(std::string infile);

private:

    std::ifstream infile;

    std::string read_instruction_from_infile();
    int16_t get_first_token(std::string instr_line);
    Instruction *parse_goal_instruction(std::string instr_line);
    Instruction *parse_wait_instruction(std::string instr_line);

    bool is_valid_instruction(Instruction *ins);

    const uint16_t POS_LOWER_BOUND = 0;
    const uint16_t POS_UPPER_BOUND = 4096;
    const uint16_t VEL_LOWER_BOUND = 0;
    const uint16_t VEL_UPPER_BOUND = 1023;
};

#endif