#include "instruction_parser.h"

InstructionParser::InstructionParser()
{
    this->current_state = InstructionState::IDLE;
}

void InstructionParser::tick()
{
    switch (this->current_state)
    {
        case InstructionState::IDLE:
        {
            // no action

            if (this->GET_NEW_INST_SET)
            {
                this->current_state = InstructionState::GET_INST;
            }
            else
            {
                this->current_state = InstructionState::IDLE;
            }

            break;
        }

        case InstructionState::GET_INST:
        {
            // read a single line, `instr_line` from `infile`

            errcode = read_instruction_from_infile();
            if (errcode && errcode != EOF_REACHED)
            {
                this->current_state = InstructionState::INVALID;
                break;
            }
            else if (errcode == EOF_REACHED) {
                this->current_state = InstructionState::IDLE;
                break;
            }
            

            int16_t first_token = get_first_token();
            if (first_token == -1)
            {
                this->current_state = InstructionState::PARSE_WAIT;
            }
            else
            {
                this->current_state = InstructionState::PARSE_GOAL;
            }

            break;
        }

        case InstructionState::PARSE_GOAL:
        {
            // parse the 6 goal positions (and 6 goal velocities) into `curr_instruction`

            errcode = parse_goal_instruction();
            if (errcode)
            {
                this->current_state = InstructionState::INVALID;
                break;
            }

            this->current_state = InstructionState::VALIDATE;

            break;
        }

        case InstructionState::PARSE_WAIT:
        {
            // parse the wait time into curr_instruction

            errcode = parse_wait_instruction();
            if (errcode)
            {
                this->current_state = InstructionState::INVALID;
                break;
            }

            this->current_state = InstructionState::VALIDATE;

            break;
        }

        case InstructionState::VALIDATE:
        {
            this->current_state = InstructionState::VALIDATE_HOLD;

            break;
        }

        case InstructionState::VALIDATE_HOLD:
        {
            // read the results of the validator

            if (is_valid_instruction)
            {
                this->current_state = InstructionState::KEEP_INST;
            }
            else
            {
                this->current_state = InstructionState::GET_INST;
            }

            break;
        }

        case InstructionState::KEEP_INST:
        {
            errcode = add_instruction_to_set();
            if (errcode)
            {
                this->current_state = InstructionState::INVALID;
                break;
            }

            this->current_state = InstructionState::GET_INST;
            break;
        }

        default:
        {
            this->current_state = InstructionState::INVALID;
            std::cerr << "Instruction Parser in invalid state! ErrCode = " << this->errcode << std::endl;
            break;
        }
    }
}

int8_t InstructionParser::read_instruction_from_infile()
{
    if (this->infile.is_open())
    {
        if(std::getline(this->infile, this->instr_line))
        {
            return InstructionErrCodes::SUCCESS;
        }
        else if (this->infile.eof())
        {
            return InstructionErrCodes::EOF_REACHED;
        }
        else
        {
            return InstructionErrCodes::GETLINE_FALED;
        }
        
    }
    else
    {
        return InstructionErrCodes::FILE_NOT_OPEN;
    }
    
}

int16_t InstructionParser::get_first_token()
{
    std::string first_token_str;
    std::stringstream ss(this->instr_line);
    ss >> first_token_str;

    return std::atoi(first_token_str.c_str());
}

int8_t InstructionParser::parse_goal_instruction()
{
    std::stringstream ss(this->instr_line);
    this->curr_instruction = new Instruction(); 
    std::string token;

    this->curr_instruction->is_wait_instr = false;

    // position parsing
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        if (ss.eof())
        {
            return InstructionErrCodes::NOT_ENOUGH_MEMBERS;
        }

        else
        {
            ss >> token;
            this->curr_instruction->goal_positions[i] = std::stoi(token);
        }
        
    }
    // velocity parsing
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        if (ss.eof())
        {
            return InstructionErrCodes::NOT_ENOUGH_MEMBERS;
        }
        else
        {
            ss >> token;
            this->curr_instruction->goal_velocities[i] = std::stoi(token);
        }
        
    }

}

int8_t InstructionParser::parse_wait_instruction()
{
    std::stringstream ss(this->instr_line);
    this->curr_instruction = new Instruction();
    std::string token;

    this->curr_instruction->is_wait_instr = true;

    // ditch the -1
    ss >> token;

    if (ss.eof())
    {
        return InstructionErrCodes::NOT_ENOUGH_MEMBERS;
    }

    ss >> token;
    this->curr_instruction->wait_time_ms = std::stoi(token);
}

int8_t InstructionParser::set_infile(std::string infile)
{
    this->infile = std::ifstream(infile);
    if (this->infile.is_open())
    {
        return InstructionErrCodes::SUCCESS;
    }
    return InstructionErrCodes::FILE_NOT_OPEN;
}

int8_t InstructionParser::set_is_valid_instruction(bool is_valid_instruction)
{
    this->is_valid_instruction = is_valid_instruction;
}

std::vector<Instruction *> InstructionParser::get_instruction_set()
{
    return this->instruction_set;
}

int8_t InstructionParser::set_get_new_inst_set(bool get_new_inst_set)
{
    this->get_instruction_set = get_new_inst_set;
    return SUCCESS;
}

int8_t InstructionParser::add_instruction_to_set()
{
    this->instruction_set.push_back(this->curr_instruction);
    return SUCCESS;
}