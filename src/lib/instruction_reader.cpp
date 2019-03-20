#include "instruction_reader.h"

Instruction::Instruction(uint16_t wait_time)
{
    this->is_goal_step = false;
    this->wait_time = wait_time;
}

Instruction::Instruction(uint16_t goal_positions[NUM_DYNAMIXELS])
{
    this->is_goal_step = true;

    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        this->goal_positions[i] = goal_positions[i];
    }

}

bool Instruction::get_is_goal_step()
{
    return this->is_goal_step;
}

uint16_t Instruction::get_wait_time()
{
    return this->wait_time;
}

uint16_t* Instruction::get_goal_positions()
{
    return this->goal_positions;
}



InstructionReader::InstructionReader(std::string config_file)
{
    this->config_file = config_file;
}

std::vector<Instruction*>& InstructionReader::get_instruction_set()
{
    // open the config file
    // read line by line
    // create new instruction per line
    // add it to the set
    // return our set

    return instruction_set;
}

void InstructionReader::destroy_instruction_set()
{
    // iterate over the set
    // destroy each instruction
}