#include "validator.h"

Validator::Validator()
{
    this->current_state = ValidatorState::IDLE;
}

void Validator::tick()
{
    switch (this->current_state)
    {
        case ValidatorState::IDLE:
        {
            this->is_valid = true;

            if (this->eval_instruction && !this->instruction.is_wait_instr)
            {
                this->current_state = ValidatorState::EVAL_GOAL;
            }

            break;
        }

        case ValidatorState::EVAL_GOAL:
        {
            evaluate_instruction();
            this->current_state = ValidatorState::IDLE;

            break;
        }

        default:
        {
            std::cerr << "Validator in invalid state! Errcode = " << errcode << std::endl;
            break;
        }
    }
}

bool Validator::get_is_valid()
{
    return this->is_valid;
}

int8_t Validator::set_instruction(Instruction *instruction)
{
    this->instruction = instruction;
}

int8_t Validator::set_eval_instruction(bool eval_instruction)
{
    this->eval_instruction = eval_instruction;
}

void Validator::evaluate_instruction()
{
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        uint16_t pos = this->instruction->goal_positions[i];
        uint16_t vel = this->instruction->goal_velocities[i];

        if (pos < POS_LOWER_BOUND || pos > POS_UPPER_BOUND)
        {
            is_valid = false;
            return;
        }
        if (vel < VEL_LOWER_BOUND || vel > VEL_UPPER_BOUND)
        {
            is_valid = false;
            return;
        }
    }

    is_valid = true;
}