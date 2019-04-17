#include "microcontroller.h"

Microcontroller::Microcontroller()
{
    this->current_state = MicrocontrollerState::INIT;

    // define shutdown idle and init
}

void Microcontroller::tick()
{
    switch (this->current_state)
    {
        case MicrocontrollerState::INIT:
        {
            this->on_led = true;
            this->insctr = 0;
            this->reinitialize = false;
            this->shutdown = false;
            this->curr_ins = initcmd;

            this->dynlib.set_instr(this->curr_ins);

            this->current_state = MicrocontrollerState::WAIT_FOR_COMP;

            break;
        }

        case MicrocontrollerState::CMD:
        {
            this->curr_ins = this->inst_set.at(this->insctr);
            this->dynlib.set_instr(this->curr_ins);
            this->insctr += 1;
            this->insctr %= this->N;
            this->curr_ins_finished = false;

            if (this->send_new_ins)
            {
                this->current_state = MicrocontrollerState::SWITCH_INST;
            }
            else
            {
                this->current_state = MicrocontrollerState::WAIT_FOR_COMP;
            }

            break;
        }

        case MicrocontrollerState::SHUTDOWN:
        {
            this->curr_ins = this->shutdown_cmd;
            this->dynlib.set_instr(this->curr_ins);
            this->dynlib.set_shutdown(true);

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

        case MicrocontrollerState::SWITCH_INST:
        {
            this->curr_ins = this->idle_cmd;
            this->dynlib.set_instr(this->curr_ins);
            this->inst_set = this->new_inst_set;
            this->insctr = 0;
            this->send_new_ins = false;

            this->current_state = MicrocontrollerState::WAIT_FOR_COMP;

            break;
        }

        case MicrocontrollerState::WAIT_FOR_COMP:
        {
            this->leg_data = this->dynlib.get_pos_data();
            get_curr_inst_state();
            
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

void Microcontroller::get_curr_inst_state()
{
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        if (abs(this->leg_data[i] - this->curr_ins.goal_positions[i]) < this->goal_tol)
        {
            this->finished[i] = true;
        }
    }

    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        if (!this->finished[i])
        {
            this->curr_ins_finished = false;
            return;
        }
    }

    this->curr_ins_finished = true;
}

int8_t Microcontroller::set_reinit_cmd(bool reinitialize)
{
    this->reinitialize = reinitialize;
    return MicrocontrollerErrorCodes::SUCCESS;
}

int8_t Microcontroller::set_shutdown_cmd(bool shutdown)
{
    this->shutdown = shutdown;
}

uint16_t* Microcontroller::get_leg_data()
{
    return this->leg_data;
}

Instruction Microcontroller::get_curr_inst()
{
    return this->curr_ins;
}

int8_t Microcontroller::set_instruction_set(std::vector<Instruction*> instruction_set)
{
    this->inst_set = instruction_set;
    return MicrocontrollerErrorCodes::SUCCESS;
}

int8_t Microcontroller::set_N(uint16_t N)
{
    this->N = N;
    return MicrocontrollerErrorCodes::SUCCESS;
}

int8_t Microcontroller::set_send_new_ins(bool send_new_ins)
{
    this->send_new_ins = send_new_ins;
    return MicrocontrollerErrorCodes::SUCCESS;
}

int8_t Microcontroller::set_new_inst_set(std::vector<Instruction*> new_inst_set)
{
    this->new_inst_set = new_inst_set;
    return MicrocontrollerErrorCodes::SUCCESS;
}
