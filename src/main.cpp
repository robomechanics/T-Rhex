#include "lib/dynamixel_interface.h"
#include "lib/instruction_reader.h"
#include <iostream>

const static int expected_arg_cnt = 2;

int main(int argc, char **argv)
{

    if (argc != expected_arg_cnt)
    {
        std::cerr << "Usage: ./run_dynamixel <config_file>"
    }

    return 0;
}