/**
 * interface file for validator
 * 
 * created by vivaan bahl, vrbahl
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <cstdint>
#include "instruction_parser/instruction_parser.h"
#include "config/config.h"

enum class ValidatorState : uint8_t
{
    IDLE,
    EVAL_GOAL,
    INVALID
};

class Validator
{
public:
    bool get_is_valid();
    int8_t set_instruction(Instruction *instruction);
    int8_t set_eval_instruction(bool eval_instruction);

    void tick();
    Validator();

private:
    ValidatorState current_state;

    bool is_valid;
    Instruction *instruction;
    bool eval_instruction;

    int8_t errcode;
    const uint16_t POS_LOWER_BOUND = 0;
    const uint16_t POS_UPPER_BOUND = 4096;
    const uint16_t VEL_LOWER_BOUND = 0;
    const uint16_t VEL_UPPER_BOUND = 1023;

    void evaluate_instruction();
};

#endif