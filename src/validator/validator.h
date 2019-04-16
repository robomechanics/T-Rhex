/**
 * interface file for validator
 * 
 * created by vivaan bahl, vrbahl
 * last modified 4/15/2019
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <cstdint>
#include "instruction_parser/instruction_parser.h"

enum ValidatorState
{
    IDLE,
    EVAL_GOAL,
    INVALID
};

class Validator
{
public:
    bool get_is_valid();
    int8_t set_instruction(Instruction instruction);
    int8_t set_eval_instruction(bool eval_instruction);

private:
    ValidatorState current_state;

    bool is_valid;
    Instruction instruction;
    bool eval_instruction;
};

#endif