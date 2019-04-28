/**
 * Main runner file for T-Rhex
 * 
 * Created by Vivaan Bahl, vrbahl
*/

#include <unistd.h>
#include <iostream>

#include "config/config.h"
#include "dynamixel_interface/dynamixel_interface.h"
#include "instruction_parser/instruction_parser.h"
#include "microcontroller/microcontroller.h"
#include "network_interface/network_interface.h"

static bool run_system = true;

void sigint_handler ( int signum )
{
    run_system = false;
    std::cout << "Stopping system..." << std::endl;

}

int main(int argc, char **argv)
{
    InstructionParser instruction_parser(gait_config_file);
    std::vector<Instruction *> instruction_set = instruction_parser.get_instruction_set();

    std::cout << "Read instruction file" << std::endl;

    DynamixelInterface dynamixel_interface;
    Microcontroller microcontroller(instruction_set);
    // NetworkInterface network_interface;

    Instruction *micr_dyni_current_instruction;
    bool micr_dyni_execute = false;

    bool dyni_micr_is_finished = false;
    uint16_t *dyni_micro_leg_data = NULL;

    std::cout << "Initialized" << std::endl;

    while (run_system)
    {
        // MICROCONTROLLER
        // send inputs
        microcontroller.set_leg_data(dyni_micro_leg_data);
        microcontroller.set_curr_ins_finished(dyni_micr_is_finished);
        // tick
        microcontroller.tick();
        // get outputs
        micr_dyni_current_instruction = microcontroller.get_curr_inst();
        micr_dyni_execute = microcontroller.get_send_instr();

        // DYNAMIXEL INTERFACE
        // send inputs
        dynamixel_interface.set_instr(micr_dyni_current_instruction);
        dynamixel_interface.set_run_command(micr_dyni_execute);
        // tick
        dynamixel_interface.tick();
        // get outputs
        dyni_micr_is_finished = dynamixel_interface.get_cmd_finished();
        dyni_micro_leg_data = dynamixel_interface.get_pos_data();


        // NETWORK INTERFACE
        // send inputs
        // tick
        // get outputs

        usleep(1 * 1000); // 1ms run loop
    }

    microcontroller.set_shutdown_cmd(true);
    dynamixel_interface.set_shutdown(true);

    while (dynamixel_interface.is_port_open())
    {
        microcontroller.tick();
        dynamixel_interface.tick();
        usleep(10 * 1000);
    }


    return 0;
}
