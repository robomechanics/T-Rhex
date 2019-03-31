#include "lib/dynamixel_interface.h"
#include "lib/instruction_reader.h"
#include <iostream>
#include <string>
#include <unistd.h>

const static int expected_arg_cnt = 2;
static bool run_instructions = true;

void sigint_handler( int signum )
{
    run_instructions = false;
}

int main(int argc, char **argv)
{

    if (argc != expected_arg_cnt)
    {
        std::cerr << "Usage: ./run_dynamixel <config_file>" << std::endl;
        return 1;
    }

    std::string filename = std::string(argv[1]);
    InstructionReader reader(filename);
    std::vector<Instruction*> instructions = reader.get_instruction_set();

    // dyn_intf_init();

    while (run_instructions)
    {
        for (auto instruction : instructions)
        {
            if (instruction->get_is_goal_step())
            {
                // run goal step code
                set_dynamixel_positions(dynamixel_ids, instruction->get_goal_positions());
                std::cout << "Goal positions: ";
                uint16_t *poses = instruction->get_goal_positions();
                for (uint8_t i = 0; i < NUM_DYNAMIXELS; i++)
                {
                    std::cout << std::to_string(poses[i]) << " ";
                }
                std::cout << std::endl;
            }
            else
            {
                // run wait code
                usleep(instruction->get_wait_time() * 1000);
                std::cout << "Slept for " << std::to_string(instruction->get_wait_time()) << "ms" << std::endl;
            }
            
        }
    }

    reader.destroy_instruction_set();

    return 0;
}