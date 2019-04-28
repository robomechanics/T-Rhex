#include "microcontroller.h"

Microcontroller::Microcontroller(std::vector<Instruction *> instruction_set)
{
    this->current_state = MicrocontrollerState::INIT;
    this->inst_set = instruction_set;
    this->N = instruction_set.size();

    // define shutdown idle and init
    uint16_t starting_goals[NUM_DYNAMIXELS] = { 0 };
    int16_t starting_velocities[NUM_DYNAMIXELS] = { 20, 20, 20, 20, 20, 20 };
    this->initcmd = new Instruction();
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        this->initcmd->goal_positions[i] = starting_goals[i];
        this->initcmd->goal_velocities[i] = starting_velocities[i];
    }

    std::cout << "Initialized Microcontroller" << std::endl;
}

void Microcontroller::tick()
{
    switch (this->current_state)
    {
        case MicrocontrollerState::INIT:
        {
#if VERBOSE
            std::cout << "Init state" << std::endl;
#endif
            this->insctr = 0;
            this->reinitialize = false;
            this->shutdown = false;
            this->curr_ins = initcmd;

            this->current_state = MicrocontrollerState::WAIT_FOR_COMP;

            break;
        }

        case MicrocontrollerState::CMD:
        {
            this->curr_ins = this->inst_set.at(this->insctr);
            this->insctr += 1;
            this->insctr %= this->N;
            this->curr_ins_finished = false;
            this->send_new_ins = true;
#if VERBOSE
            std::cout << "Running command number " << this->insctr << std::endl;
#endif

            this->current_state = MicrocontrollerState::WAIT_FOR_COMP;

            break;
        }

        case MicrocontrollerState::SHUTDOWN:
        {
#if VERBOSE
            std::cout << "Shutting down" << std::endl;
#endif
            this->current_state = MicrocontrollerState::IDLE;

            break;
        }

        case MicrocontrollerState::IDLE:
        {
            // turn the led off
            if (this->reinitialize)
            {
                this->current_state = MicrocontrollerState::INIT;
            }

            break;
        }

        case MicrocontrollerState::WAIT_FOR_COMP:
        {
            if (this->curr_ins_finished)
            {
                this->current_state = MicrocontrollerState::CMD;
            }

            break;
        }

        default:
        {
            std::cerr << "Microcontroller in invalid state! Errcode = %d" << errcode << std::endl;
            break;
        }
    }
}

bool Microcontroller::get_send_instr()
{
    return this->send_new_ins;
}

void Microcontroller::set_curr_ins_finished(bool is_finished)
{
    this->curr_ins_finished = is_finished;
}

int8_t Microcontroller::set_reinit_cmd(bool reinitialize)
{
    this->reinitialize = reinitialize;
    return 0;
}

int8_t Microcontroller::set_shutdown_cmd(bool shutdown)
{
    this->shutdown = shutdown;
    return 0;
}

uint16_t* Microcontroller::get_leg_data()
{
    return this->leg_data;
}

Instruction* Microcontroller::get_curr_inst()
{
    return this->curr_ins;
}

void Microcontroller::set_leg_data(uint16_t leg_data[NUM_DYNAMIXELS])
{
    //TODO
    return;
}
