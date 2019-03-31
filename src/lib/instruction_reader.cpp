#include "instruction_reader.h"

Instruction::Instruction(uint16_t wait_time_ms)
{
    this->is_goal_step = false;
    this->wait_time_ms = wait_time_ms;
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
    return this->wait_time_ms;
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
    std::ifstream input_stream(this->config_file);
    std::string line;

    // read line by line
    while (getline(input_stream, line))
    {
        // create new instruction per line
        std::vector<std::string> tokens;
        std::string buf;
        std::stringstream linebuf(line);
        
        while (linebuf >> buf)
        {
            tokens.push_back(buf);
        }

        Instruction *step;

        int16_t first_entry = std::stoi(tokens.at(0));
        if (first_entry == -1)
        {
            uint16_t wait_time_ms = std::stoi(tokens.at(1));
            step = new Instruction(wait_time_ms);
        }
        else
        {
            uint16_t goal_positions[NUM_DYNAMIXELS];
            for (int i = 0; i < NUM_DYNAMIXELS; i++)
            {
                goal_positions[i] = std::stoi(tokens.at(i));
            }

            step = new Instruction(goal_positions);
        }

        // add it to the set
        instruction_set.push_back(step);

    }

    return instruction_set;
}

void InstructionReader::destroy_instruction_set()
{
    // iterate over the set
    for (Instruction *i : this->instruction_set)
    {
        // destroy each instruction
        delete i;
    }
}