#include "instruction_parser.h"

InstructionParser::InstructionParser(std::string infile)
{
    this->infile = std::ifstream(infile);
}

std::vector<Instruction *> InstructionParser::get_instruction_set()
{
    std::vector<Instruction *> instruction_set;
    int8_t stat_code = 0;
    while (!stat_code)
    {
        Instruction *curr_ins;
        std::string line = read_instruction_from_infile();

        if (line == "eof")
        {
            break;
        }

        if (get_first_token(line) == -1)
        {
            curr_ins = parse_wait_instruction(line);
        }
        else
        {
            curr_ins = parse_goal_instruction(line);
        }

        if (is_valid_instruction(curr_ins))
        {
            instruction_set.push_back(curr_ins);
        }
        
    }

    return instruction_set;
}

bool InstructionParser::is_valid_instruction(Instruction *ins)
{
    if (!ins->is_wait_instr)
    {
        for (int i = 0; i < NUM_DYNAMIXELS; i++)
        {
            uint16_t pos = ins->goal_positions[i];

            if (pos < POS_LOWER_BOUND || pos > POS_UPPER_BOUND)
            {
                return false;
            }
        }
        for (int i = 0; i < NUM_LEGS; i++)
        {
            uint16_t vel = ins->goal_velocities[i];
            if (vel < VEL_LOWER_BOUND || vel > VEL_UPPER_BOUND)
            {
                return false;
            }
        }
    }
    return true;
}

std::string InstructionParser::read_instruction_from_infile()
{
    std::string instr_line;

    if (this->infile.is_open())
    {
        if(std::getline(this->infile, instr_line))
        {
            return instr_line;
        }
        else if (this->infile.eof())
        {
            return "eof";
        }
        else
        {
            return "";
        }
        
    }
    else
    {
        return "";
    }
    
}

int16_t InstructionParser::get_first_token(std::string instr_line)
{
    std::string first_token_str;
    std::stringstream ss(instr_line);
    ss >> first_token_str;

    return std::atoi(first_token_str.c_str());
}

Instruction* InstructionParser::parse_goal_instruction(std::string instr_line)
{
    std::stringstream ss(instr_line);
    Instruction *curr_instruction = new Instruction(); 
    std::string token;

    curr_instruction->is_wait_instr = false;

    // position parsing
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        if (ss.eof())
        {
            delete curr_instruction;
            return NULL;
        }

        else
        {
            ss >> token;
            curr_instruction->goal_positions[i] = std::stoi(token);
        }
        
    }
    // velocity parsing
    for (int i = 0; i < NUM_LEGS ; i++)
    {
        if (ss.eof())
        {
            delete curr_instruction;
            return NULL;
        }
        else
        {
            ss >> token;
            curr_instruction->goal_velocities[i] = std::stoi(token);
        }
        
    }

    return curr_instruction;

}

Instruction* InstructionParser::parse_wait_instruction(std::string instr_line)
{
    std::stringstream ss(instr_line);
    Instruction *curr_instruction = new Instruction();
    std::string token;

    curr_instruction->is_wait_instr = true;

    // ditch the -1
    ss >> token;

    if (ss.eof())
    {
        delete curr_instruction;
        return NULL;
    }

    ss >> token;
    curr_instruction->wait_time_ms = std::stoi(token);

    return curr_instruction;
}
