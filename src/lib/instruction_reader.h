#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#ifndef INSTR_RDR_H
#define INSTR_RDR_H

const static uint8_t NUM_DYNAMIXELS = 6;

class Instruction
{
public:
    Instruction(uint16_t wait_time_ms);
    Instruction(uint16_t goal_positions[NUM_DYNAMIXELS], uint16_t goal_velocities[NUM_DYNAMIXELS]);

    uint16_t get_wait_time();
    bool get_is_goal_step();
    uint16_t *get_goal_positions();
    uint16_t *get_goal_velocities();

private:
    bool is_goal_step;
    uint16_t wait_time_ms;
    uint16_t goal_positions[NUM_DYNAMIXELS];
    uint16_t goal_velocities[NUM_DYNAMIXELS];
};

class InstructionReader
{
public:
    InstructionReader(std::string config_file);
    std::vector<Instruction*>& get_instruction_set();
    void destroy_instruction_set();
private:
    std::vector<Instruction *> instruction_set;
    std::string config_file;
};


#endif