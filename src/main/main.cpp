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
#include "validator/validator.h"

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

    DynamixelInterface dynamixel_interface;
    Microcontroller microcontroller;
    NetworkInterface network_interface;

    std::vector<Instruction *> instruction_set;

    while (run_system)
    {
        // MICROCONTROLLER
        // send inputs
        // tick
        // get outputs

        // DYNAMIXEL INTERFACE
        // send inputs
        // tick
        // get outputs

        // NETWORK INTERFACE
        // send inputs
        // tick
        // get outputs
    }

    return 0;
}